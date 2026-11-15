#include "BitmapFontLoader.hpp"

#include <yyjson.h>

bool load_bitmap_font_data(const std::string& font_name,
                           Texture2D& out_tex,
                           std::unordered_map<int32_t, BitmapGlyph>& out_glyphs,
                           float& out_line_height) {
    if (font_name.empty() || font_name.find('.') != std::string::npos) {
        return false;
    }

    yyjson_doc* doc = yyjson_read_file("assets/fonts.json", 0, nullptr, nullptr);
    if (!doc) {
        return false;
    }

    yyjson_val* root = yyjson_doc_get_root(doc);
    if (!yyjson_is_arr(root)) {
        yyjson_doc_free(doc);
        return false;
    }

    yyjson_val* font_obj = nullptr;
    std::size_t idx = 0;
    std::size_t max = 0;
    yyjson_val* item = nullptr;
    yyjson_arr_foreach(root, idx, max, item) {
        const char* name = yyjson_get_str(yyjson_obj_get(item, "name"));
        if (name && font_name == name) {
            font_obj = item;
            break;
        }
    }

    if (!font_obj) {
        yyjson_doc_free(doc);
        return false;
    }

    yyjson_val* tex_page_val = yyjson_obj_get(font_obj, "texturePage");
    yyjson_val* tex_x_val = yyjson_obj_get(font_obj, "textureX");
    yyjson_val* tex_y_val = yyjson_obj_get(font_obj, "textureY");
    yyjson_val* glyphs_arr = yyjson_obj_get(font_obj, "glyphs");
    if (!yyjson_is_int(tex_page_val) || !yyjson_is_int(tex_x_val) || !yyjson_is_int(tex_y_val) ||
        !yyjson_is_arr(glyphs_arr)) {
        yyjson_doc_free(doc);
        return false;
    }

    const int32_t tex_page = static_cast<int32_t>(yyjson_get_sint(tex_page_val));
    const int32_t tex_x = static_cast<int32_t>(yyjson_get_sint(tex_x_val));
    const int32_t tex_y = static_cast<int32_t>(yyjson_get_sint(tex_y_val));

    const std::string tex_path = "assets/EmbeddedTextures/Texture " + std::to_string(tex_page) + ".png";
    out_tex = LoadTexture(tex_path.c_str());
    if (out_tex.id == 0) {
        yyjson_doc_free(doc);
        return false;
    }

    SetTextureFilter(out_tex, TEXTURE_FILTER_POINT);

    std::size_t g_idx = 0;
    std::size_t g_max = 0;
    yyjson_val* g_item = nullptr;
    yyjson_arr_foreach(glyphs_arr, g_idx, g_max, g_item) {
        yyjson_val* ch_val = yyjson_obj_get(g_item, "character");
        yyjson_val* sx_val = yyjson_obj_get(g_item, "sourceX");
        yyjson_val* sy_val = yyjson_obj_get(g_item, "sourceY");
        yyjson_val* sw_val = yyjson_obj_get(g_item, "sourceWidth");
        yyjson_val* sh_val = yyjson_obj_get(g_item, "sourceHeight");
        yyjson_val* shift_val = yyjson_obj_get(g_item, "shift");
        yyjson_val* offset_val = yyjson_obj_get(g_item, "offset");
        if (!ch_val || !sx_val || !sy_val || !sw_val || !sh_val || !shift_val || !offset_val) {
            continue;
        }

        const int32_t ch = static_cast<int32_t>(yyjson_get_sint(ch_val));
        const int32_t sx = static_cast<int32_t>(yyjson_get_sint(sx_val));
        const int32_t sy = static_cast<int32_t>(yyjson_get_sint(sy_val));
        const int32_t sw = static_cast<int32_t>(yyjson_get_sint(sw_val));
        const int32_t sh = static_cast<int32_t>(yyjson_get_sint(sh_val));
        const int32_t shift = static_cast<int32_t>(yyjson_get_sint(shift_val));
        const int32_t offset = static_cast<int32_t>(yyjson_get_sint(offset_val));

        out_glyphs[ch] = BitmapGlyph{
            static_cast<float>(tex_x + sx),
            static_cast<float>(tex_y + sy),
            static_cast<float>(sw),
            static_cast<float>(sh),
            static_cast<float>(shift),
            static_cast<float>(offset),
        };
        if (sh > 0) {
            out_line_height = static_cast<float>(sh) + 2.0f;
        }
    }

    yyjson_doc_free(doc);
    return true;
}
