#include "BackgroundAtlas.hpp"

#include <yyjson.h>

BackgroundAtlas::~BackgroundAtlas() {
    for (auto& [page, tex] : pages_) {
        if (tex.id > 0) {
            UnloadTexture(tex);
        }
    }
    pages_.clear();
}

bool BackgroundAtlas::load(const std::string& json_path) {
    yyjson_doc* doc = yyjson_read_file(json_path.c_str(), 0, nullptr, nullptr);
    if (!doc) {
        return false;
    }

    yyjson_val* root = yyjson_doc_get_root(doc);
    if (yyjson_is_arr(root)) {
        size_t idx = 0;
        size_t max = 0;
        yyjson_val* item = nullptr;
        yyjson_arr_foreach(root, idx, max, item) {
            const char* name = yyjson_get_str(yyjson_obj_get(item, "name"));
            yyjson_val* tex = yyjson_obj_get(item, "texture");
            if (!name || !tex) {
                continue;
            }
            BackgroundEntry entry;
            entry.texture_page = yyjson_get_int(yyjson_obj_get(tex, "texturePage"));
            entry.source_x = static_cast<float>(yyjson_get_num(yyjson_obj_get(tex, "sourceX")));
            entry.source_y = static_cast<float>(yyjson_get_num(yyjson_obj_get(tex, "sourceY")));
            entry.source_width = static_cast<float>(yyjson_get_num(yyjson_obj_get(tex, "sourceWidth")));
            entry.source_height = static_cast<float>(yyjson_get_num(yyjson_obj_get(tex, "sourceHeight")));
            entries_[name] = entry;
        }
    }

    yyjson_doc_free(doc);
    return !entries_.empty();
}

const BackgroundEntry* BackgroundAtlas::find(const std::string& name) const {
    auto it = entries_.find(name);
    return it != entries_.end() ? &it->second : nullptr;
}

Texture2D BackgroundAtlas::texture_for_page(int32_t page) {
    auto it = pages_.find(page);
    if (it != pages_.end()) {
        return it->second;
    }

    std::string path = "assets/EmbeddedTextures/Texture " + std::to_string(page) + ".png";
    Texture2D tex = LoadTexture(path.c_str());
    pages_[page] = tex;
    return tex;
}
