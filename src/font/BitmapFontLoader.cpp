#include "BitmapFontLoader.hpp"

#include <cstdlib>
#include <fstream>
#include <sstream>

namespace {

bool parse_int_field(const std::string& src, const std::string& key, std::size_t from, std::size_t limit, int& out) {
    std::size_t pos = src.find("\"" + key + "\":", from);
    if (pos == std::string::npos || pos >= limit) {
        return false;
    }
    pos += key.size() + 3;
    out = std::atoi(src.c_str() + pos);
    return true;
}

}  // namespace

bool load_bitmap_font_data(const std::string& font_name,
                           Texture2D& out_tex,
                           std::unordered_map<int, BitmapGlyph>& out_glyphs,
                           float& out_line_height) {
    if (font_name.empty() || font_name.find('.') != std::string::npos) {
        return false;
    }

    std::ifstream file("assets/fonts.json");
    if (!file) {
        return false;
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    const std::string json = ss.str();

    const std::string needle = "\"name\": \"" + font_name + "\"";
    std::size_t font_pos = json.find(needle);
    if (font_pos == std::string::npos) {
        return false;
    }

    std::size_t glyphs_pos = json.find("\"glyphs\": [", font_pos);
    std::size_t glyphs_end = json.find("]", glyphs_pos);
    int tex_page = 0;
    int tex_x = 0;
    int tex_y = 0;
    if (glyphs_pos == std::string::npos || glyphs_end == std::string::npos ||
        !parse_int_field(json, "texturePage", font_pos, glyphs_pos, tex_page) ||
        !parse_int_field(json, "textureX", font_pos, glyphs_pos, tex_x) ||
        !parse_int_field(json, "textureY", font_pos, glyphs_pos, tex_y)) {
        return false;
    }

    std::string tex_path = "assets/EmbeddedTextures/Texture " + std::to_string(tex_page) + ".png";
    out_tex = LoadTexture(tex_path.c_str());
    if (out_tex.id == 0) {
        return false;
    }

    SetTextureFilter(out_tex, TEXTURE_FILTER_POINT);

    std::size_t cur = glyphs_pos;
    while (cur < glyphs_end) {
        std::size_t obj_start = json.find('{', cur);
        if (obj_start == std::string::npos || obj_start >= glyphs_end) {
            break;
        }
        std::size_t obj_end = json.find('}', obj_start);
        if (obj_end == std::string::npos || obj_end > glyphs_end) {
            break;
        }

        int ch = 0, sx = 0, sy = 0, sw = 0, sh = 0, shift = 0, offset = 0;
        if (parse_int_field(json, "character", obj_start, obj_end, ch) &&
            parse_int_field(json, "sourceX", obj_start, obj_end, sx) &&
            parse_int_field(json, "sourceY", obj_start, obj_end, sy) &&
            parse_int_field(json, "sourceWidth", obj_start, obj_end, sw) &&
            parse_int_field(json, "sourceHeight", obj_start, obj_end, sh) &&
            parse_int_field(json, "shift", obj_start, obj_end, shift) &&
            parse_int_field(json, "offset", obj_start, obj_end, offset)) {
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
        cur = obj_end + 1;
    }

    return true;
}
