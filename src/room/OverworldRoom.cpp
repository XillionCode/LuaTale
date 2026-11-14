#include "OverworldRoom.hpp"
#include "entity/Entity.hpp"
#include "entity/PlayerEntity.hpp"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <unordered_map>
#include <yyjson.h>

namespace {

Color unpack_gm_color(uint32_t argb) {
    unsigned char a = static_cast<unsigned char>((argb >> 24) & 0xFF);
    unsigned char b = static_cast<unsigned char>((argb >> 16) & 0xFF);
    unsigned char g = static_cast<unsigned char>((argb >> 8) & 0xFF);
    unsigned char r = static_cast<unsigned char>(argb & 0xFF);
    return Color{r, g, b, a};
}

Vector2 load_sprite_size(const std::string& sprite, std::unordered_map<std::string, Vector2>& cache) {
    auto it = cache.find(sprite);
    if (it != cache.end()) {
        return it->second;
    }
    std::string path = "assets/Sprites/" + sprite + "_0.png";
    Image img = LoadImage(path.c_str());
    Vector2 size = img.data ? Vector2{static_cast<float>(img.width), static_cast<float>(img.height)}
                            : Vector2{20.0f, 20.0f};
    if (img.data) {
        UnloadImage(img);
    }
    cache[sprite] = size;
    return size;
}

std::unordered_map<std::string, std::string> load_object_sprites(const char* path) {
    std::unordered_map<std::string, std::string> map;
    yyjson_doc* doc = yyjson_read_file(path, 0, nullptr, nullptr);
    if (!doc) {
        return map;
    }
    yyjson_val* root = yyjson_doc_get_root(doc);
    if (yyjson_is_arr(root)) {
        size_t idx = 0;
        size_t max = 0;
        yyjson_val* item = nullptr;
        yyjson_arr_foreach(root, idx, max, item) {
            const char* name = yyjson_get_str(yyjson_obj_get(item, "name"));
            const char* spr = yyjson_get_str(yyjson_obj_get(item, "sprite"));
            if (name && spr) {
                map[name] = spr;
            }
        }
    }
    yyjson_doc_free(doc);
    return map;
}

bool collides_slope(const Rectangle& hb, const SlopeWall& s) {
    if (!CheckCollisionRecs(hb, s.rect)) {
        return false;
    }
    const float rx = s.rect.x;
    const float ry = s.rect.y;
    const float rw = s.rect.width;
    switch (s.type) {
        case SlopeWall::SUL:
            return (hb.x - rx) + (hb.y - ry) < rw;
        case SlopeWall::SUR:
            return (hb.y - ry) < (hb.x + hb.width - rx);
        case SlopeWall::SDL:
            return (hb.x - rx) < (hb.y + hb.height - ry);
        case SlopeWall::SDR:
            return (hb.x + hb.width - rx) + (hb.y + hb.height - ry) > rw;
    }
    return false;
}

}  // namespace

OverworldRoom::OverworldRoom(const std::string& json_path) {
    camera_.zoom = 2.0f;
    atlas_.load("assets/backgrounds.json");
    load_room(json_path);
}

OverworldRoom::~OverworldRoom() = default;

bool OverworldRoom::load_room(const std::string& json_path) {
    yyjson_doc* doc = yyjson_read_file(json_path.c_str(), 0, nullptr, nullptr);
    if (!doc) {
        return false;
    }

    yyjson_val* root = yyjson_doc_get_root(doc);
    room_width_ = static_cast<float>(yyjson_get_num(yyjson_obj_get(root, "width")));
    room_height_ = static_cast<float>(yyjson_get_num(yyjson_obj_get(root, "height")));

    yyjson_val* bgs = yyjson_obj_get(root, "backgrounds");
    if (yyjson_is_arr(bgs)) {
        size_t idx = 0;
        size_t max = 0;
        yyjson_val* item = nullptr;
        yyjson_arr_foreach(bgs, idx, max, item) {
            if (!yyjson_get_bool(yyjson_obj_get(item, "enabled"))) {
                continue;
            }
            const char* def = yyjson_get_str(yyjson_obj_get(item, "backgroundDefinition"));
            if (!def || def[0] == '\0') {
                continue;
            }
            RoomBackground bg;
            bg.foreground = yyjson_get_bool(yyjson_obj_get(item, "foreground"));
            bg.definition = def;
            bg.x = static_cast<float>(yyjson_get_num(yyjson_obj_get(item, "x")));
            bg.y = static_cast<float>(yyjson_get_num(yyjson_obj_get(item, "y")));
            bg.tile_x = yyjson_get_bool(yyjson_obj_get(item, "tileX"));
            bg.tile_y = yyjson_get_bool(yyjson_obj_get(item, "tileY"));
            backgrounds_.push_back(std::move(bg));
        }
    }

    yyjson_val* tiles = yyjson_obj_get(root, "tiles");
    if (yyjson_is_arr(tiles)) {
        size_t idx = 0;
        size_t max = 0;
        yyjson_val* item = nullptr;
        yyjson_arr_foreach(tiles, idx, max, item) {
            const char* bg = yyjson_get_str(yyjson_obj_get(item, "background"));
            if (!bg) {
                continue;
            }
            RoomTile tile;
            tile.x = static_cast<float>(yyjson_get_num(yyjson_obj_get(item, "x")));
            tile.y = static_cast<float>(yyjson_get_num(yyjson_obj_get(item, "y")));
            tile.background = bg;
            tile.source_x = static_cast<float>(yyjson_get_num(yyjson_obj_get(item, "sourceX")));
            tile.source_y = static_cast<float>(yyjson_get_num(yyjson_obj_get(item, "sourceY")));
            tile.width = static_cast<float>(yyjson_get_num(yyjson_obj_get(item, "width")));
            tile.height = static_cast<float>(yyjson_get_num(yyjson_obj_get(item, "height")));
            tile.depth = yyjson_get_int(yyjson_obj_get(item, "depth"));
            tile.scale_x = static_cast<float>(yyjson_get_num(yyjson_obj_get(item, "scaleX")));
            tile.scale_y = static_cast<float>(yyjson_get_num(yyjson_obj_get(item, "scaleY")));
            yyjson_val* col = yyjson_obj_get(item, "color");
            if (col) {
                tile.tint = unpack_gm_color(static_cast<uint32_t>(yyjson_get_uint(col)));
            }
            tiles_.push_back(std::move(tile));
        }
    }

    auto obj_sprites = load_object_sprites("assets/objects.json");
    std::unordered_map<std::string, Vector2> size_cache;

    yyjson_val* instances = yyjson_obj_get(root, "instances");
    if (yyjson_is_arr(instances)) {
        size_t idx = 0;
        size_t max = 0;
        yyjson_val* item = nullptr;
        yyjson_arr_foreach(instances, idx, max, item) {
            const char* obj = yyjson_get_str(yyjson_obj_get(item, "objectDefinition"));
            if (!obj) {
                continue;
            }
            const float x = static_cast<float>(yyjson_get_num(yyjson_obj_get(item, "x")));
            const float y = static_cast<float>(yyjson_get_num(yyjson_obj_get(item, "y")));
            const float sx = static_cast<float>(yyjson_get_num(yyjson_obj_get(item, "scaleX")));
            const float sy = static_cast<float>(yyjson_get_num(yyjson_obj_get(item, "scaleY")));

            if (std::strcmp(obj, "obj_mainchara") == 0) {
                spawn_x_ = x;
                spawn_y_ = y;
                continue;
            }

            auto spr_it = obj_sprites.find(obj);
            const std::string& spr = (spr_it != obj_sprites.end()) ? spr_it->second : std::string();
            const Vector2 base = spr.empty() ? Vector2{20.0f, 20.0f} : load_sprite_size(spr, size_cache);
            const Rectangle rect{x, y, base.x * sx, base.y * sy};

            if (std::strcmp(obj, "obj_solidsmall") == 0) {
                solid_walls_.push_back(rect);
            } else if (std::strcmp(obj, "obj_solidlong") == 0) {
                solid_walls_.push_back(Rectangle{rect.x, rect.y, rect.width * 400.0f, rect.height});
            } else if (std::strcmp(obj, "obj_solidtall") == 0) {
                solid_walls_.push_back(Rectangle{rect.x, rect.y, rect.width, rect.height * 400.0f});
            } else if (std::strcmp(obj, "obj_sul") == 0) {
                slope_walls_.push_back({SlopeWall::SUL, rect});
            } else if (std::strcmp(obj, "obj_sur") == 0) {
                slope_walls_.push_back({SlopeWall::SUR, rect});
            } else if (std::strcmp(obj, "obj_sdl") == 0) {
                slope_walls_.push_back({SlopeWall::SDL, rect});
            } else if (std::strcmp(obj, "obj_sdr") == 0) {
                slope_walls_.push_back({SlopeWall::SDR, rect});
            }
        }
    }

    yyjson_doc_free(doc);

    std::stable_sort(tiles_.begin(), tiles_.end(), [](const RoomTile& a, const RoomTile& b) {
        return a.depth > b.depth;
    });
    return true;
}

void OverworldRoom::set_follow(std::shared_ptr<PlayerEntity> target) {
    follow_target_ = target;
    if (target) {
        target->set_bounds(room_width_, room_height_);
        target->set_collider([this](Rectangle hb) { return collides(hb); });
    }
}

bool OverworldRoom::collides(Rectangle hitbox) const {
    for (const Rectangle& wall : solid_walls_) {
        if (CheckCollisionRecs(hitbox, wall)) {
            return true;
        }
    }
    for (const SlopeWall& slope : slope_walls_) {
        if (collides_slope(hitbox, slope)) {
            return true;
        }
    }
    return false;
}

float OverworldRoom::spawn_x() const {
    return spawn_x_;
}

float OverworldRoom::spawn_y() const {
    return spawn_y_;
}

float OverworldRoom::width() const {
    return room_width_;
}

float OverworldRoom::height() const {
    return room_height_;
}

void OverworldRoom::step() {
    Room::step();

    if (auto target = follow_target_.lock()) {
        const float cx = target->x() + target->width() * 0.5f;
        const float cy = target->y() + target->height() * 0.5f;
        const float max_x = std::max(0.0f, room_width_ - 320.0f);
        const float max_y = std::max(0.0f, room_height_ - 240.0f);
        camera_.target.x = std::clamp(cx - 160.0f, 0.0f, max_x);
        camera_.target.y = std::clamp(cy - 120.0f, 0.0f, max_y);
    }
}

void OverworldRoom::draw_background_layer(const RoomBackground& layer) {
    const BackgroundEntry* bg = atlas_.find(layer.definition);
    if (!bg || bg->texture_page < 0) {
        return;
    }
    Texture2D tex = atlas_.texture_for_page(bg->texture_page);
    if (tex.id == 0) {
        return;
    }
    Rectangle src{bg->source_x, bg->source_y, bg->source_width, bg->source_height};
    const float step_x = bg->source_width > 0.0f ? bg->source_width : 1.0f;
    const float step_y = bg->source_height > 0.0f ? bg->source_height : 1.0f;
    const float end_x = layer.tile_x ? room_width_ : layer.x + 1.0f;
    const float end_y = layer.tile_y ? room_height_ : layer.y + 1.0f;

    for (float y = layer.y; y < end_y; y += step_y) {
        for (float x = layer.x; x < end_x; x += step_x) {
            Rectangle dst{x, y, bg->source_width, bg->source_height};
            DrawTexturePro(tex, src, dst, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
            if (!layer.tile_x) {
                break;
            }
        }
        if (!layer.tile_y) {
            break;
        }
    }
}

void OverworldRoom::draw() {
    ClearBackground(BLACK);
    BeginMode2D(camera_);

    for (const RoomBackground& layer : backgrounds_) {
        if (!layer.foreground) {
            draw_background_layer(layer);
        }
    }

    for (const RoomTile& tile : tiles_) {
        const BackgroundEntry* bg = atlas_.find(tile.background);
        if (!bg || bg->texture_page < 0) {
            continue;
        }
        Texture2D tex = atlas_.texture_for_page(bg->texture_page);
        if (tex.id == 0) {
            continue;
        }
        Rectangle src{
            bg->source_x + tile.source_x,
            bg->source_y + tile.source_y,
            tile.width,
            tile.height,
        };
        Rectangle dst{
            tile.x,
            tile.y,
            tile.width * tile.scale_x,
            tile.height * tile.scale_y,
        };
        DrawTexturePro(tex, src, dst, Vector2{0.0f, 0.0f}, 0.0f, tile.tint);
    }

    for (std::size_t i = 0; i < entities_.size(); ++i) {
        entities_[i]->draw();
    }

    for (const RoomBackground& layer : backgrounds_) {
        if (layer.foreground) {
            draw_background_layer(layer);
        }
    }

    EndMode2D();
}
