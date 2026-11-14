#pragma once

#include <raylib.h>
#include <string>
#include <unordered_map>

struct BitmapGlyph {
    float src_x = 0.0f;
    float src_y = 0.0f;
    float src_w = 0.0f;
    float src_h = 0.0f;
    float shift = 0.0f;
    float offset = 0.0f;
};

bool load_bitmap_font_data(const std::string& font_name,
                           Texture2D& out_tex,
                           std::unordered_map<int, BitmapGlyph>& out_glyphs,
                           float& out_line_height);
