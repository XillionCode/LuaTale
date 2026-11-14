#include "BitmapFont.hpp"

namespace {

int next_utf8(const std::string& s, std::size_t& i) {
    unsigned char c0 = static_cast<unsigned char>(s[i]);
    if (c0 < 0x80) {
        ++i;
        return c0;
    }
    if ((c0 & 0xE0) == 0xC0 && i + 1 < s.size()) {
        int cp = ((c0 & 0x1F) << 6) | (static_cast<unsigned char>(s[i + 1]) & 0x3F);
        i += 2;
        return cp;
    }
    if ((c0 & 0xF0) == 0xE0 && i + 2 < s.size()) {
        int cp = ((c0 & 0x0F) << 12) |
                 ((static_cast<unsigned char>(s[i + 1]) & 0x3F) << 6) |
                 (static_cast<unsigned char>(s[i + 2]) & 0x3F);
        i += 3;
        return cp;
    }
    if ((c0 & 0xF8) == 0xF0 && i + 3 < s.size()) {
        int cp = ((c0 & 0x07) << 18) |
                 ((static_cast<unsigned char>(s[i + 1]) & 0x3F) << 12) |
                 ((static_cast<unsigned char>(s[i + 2]) & 0x3F) << 6) |
                 (static_cast<unsigned char>(s[i + 3]) & 0x3F);
        i += 4;
        return cp;
    }
    ++i;
    return c0;
}

}  // namespace

BitmapFont::~BitmapFont() {
    unload();
}

void BitmapFont::unload() {
    if (loaded_) {
        UnloadTexture(texture_);
        loaded_ = false;
        glyphs_.clear();
    }
}

bool BitmapFont::is_loaded() const {
    return loaded_;
}

bool BitmapFont::load(const std::string& font_name) {
    unload();
    loaded_ = load_bitmap_font_data(font_name, texture_, glyphs_, line_height_);
    return loaded_;
}

void BitmapFont::draw_text(const std::string& text,
                           float x,
                           float y,
                           float scale,
                           float hspace,
                           float vspace,
                           bool centered,
                           bool shake,
                           Color color,
                           float rotation) const {
    if (!loaded_) {
        return;
    }

    float cursor_y = y;
    std::size_t line_start = 0;
    while (line_start <= text.size()) {
        std::size_t line_end = text.find('\n', line_start);
        if (line_end == std::string::npos) {
            line_end = text.size();
        }

        float line_width = 0.0f;
        if (centered) {
            for (std::size_t i = line_start; i < line_end;) {
                int cp = next_utf8(text, i);
                if (hspace > 0.0f) {
                    line_width += hspace;
                } else {
                    auto it = glyphs_.find(cp);
                    if (it != glyphs_.end()) {
                        line_width += it->second.shift * scale;
                    }
                }
            }
        }

        float cursor_x = centered ? (x - line_width * 0.5f) : x;
        for (std::size_t i = line_start; i < line_end;) {
            int cp = next_utf8(text, i);
            auto it = glyphs_.find(cp);
            if (it != glyphs_.end()) {
                const BitmapGlyph& g = it->second;
                float dx = shake ? static_cast<float>(GetRandomValue(-1, 1)) : 0.0f;
                float dy = shake ? static_cast<float>(GetRandomValue(-1, 1)) : 0.0f;
                Rectangle src{g.src_x, g.src_y, g.src_w, g.src_h};
                Rectangle dst{x, y, g.src_w * scale, g.src_h * scale};
                Vector2 origin{-(cursor_x + g.offset * scale + dx - x), -(cursor_y + dy - y)};
                DrawTexturePro(texture_, src, dst, origin, rotation, color);
                cursor_x += (hspace > 0.0f) ? hspace : (g.shift * scale);
            }
        }

        if (line_end == text.size()) {
            break;
        }
        cursor_y += (vspace > 0.0f) ? vspace : (line_height_ * scale);
        line_start = line_end + 1;
    }
}
