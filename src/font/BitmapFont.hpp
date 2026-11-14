#pragma once

#include "BitmapFontLoader.hpp"

#include <raylib.h>
#include <string>
#include <unordered_map>

class BitmapFont {
public:
    BitmapFont() = default;
    ~BitmapFont();

    BitmapFont(const BitmapFont&) = delete;
    BitmapFont& operator=(const BitmapFont&) = delete;

    bool load(const std::string& font_name);
    void unload();
    bool is_loaded() const;

    void draw_text(const std::string& text,
                   float x,
                   float y,
                   float scale,
                   float hspace,
                   float vspace,
                   bool centered,
                   bool shake,
                   Color color,
                   float rotation = 0.0f) const;

private:
    Texture2D texture_{};
    bool loaded_ = false;
    std::unordered_map<int, BitmapGlyph> glyphs_;
    float line_height_ = 16.0f;
};
