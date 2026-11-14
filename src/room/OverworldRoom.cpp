#include "OverworldRoom.hpp"
#include "entity/Entity.hpp"

#include <algorithm>
#include <cstdint>
#include <yyjson.h>

namespace {

Color unpack_gm_color(uint32_t argb) {
    unsigned char a = static_cast<unsigned char>((argb >> 24) & 0xFF);
    unsigned char b = static_cast<unsigned char>((argb >> 16) & 0xFF);
    unsigned char g = static_cast<unsigned char>((argb >> 8) & 0xFF);
    unsigned char r = static_cast<unsigned char>(argb & 0xFF);
    return Color{r, g, b, a};
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

    yyjson_doc_free(doc);

    std::stable_sort(tiles_.begin(), tiles_.end(), [](const RoomTile& a, const RoomTile& b) {
        return a.depth > b.depth;
    });
    return true;
}

void OverworldRoom::step() {
    Room::step();

    const float pan_speed = 4.0f;
    if (IsKeyDown(KEY_RIGHT)) {
        camera_.target.x += pan_speed;
    }
    if (IsKeyDown(KEY_LEFT)) {
        camera_.target.x -= pan_speed;
    }
    if (IsKeyDown(KEY_DOWN)) {
        camera_.target.y += pan_speed;
    }
    if (IsKeyDown(KEY_UP)) {
        camera_.target.y -= pan_speed;
    }

    const float max_x = std::max(0.0f, room_width_ - 320.0f);
    const float max_y = std::max(0.0f, room_height_ - 240.0f);
    camera_.target.x = std::clamp(camera_.target.x, 0.0f, max_x);
    camera_.target.y = std::clamp(camera_.target.y, 0.0f, max_y);
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
