#include "TextEntity.hpp"

TextEntity::TextEntity(const std::string& text,
                       float x,
                       float y,
                       const std::string& font_path,
                       const std::string& sfx_path,
                       float font_size,
                       int steps_per_char)
    : text_(text),
      x_(x),
      y_(y),
      font_size_(font_size),
      steps_per_char_(steps_per_char > 0 ? steps_per_char : 1) {
    set_font(font_path);

    if (!sfx_path.empty()) {
        sfx_ = LoadSound(sfx_path.c_str());
        has_sfx_ = sfx_.frameCount > 0;
    }
}

TextEntity::~TextEntity() {
    if (custom_font_) {
        UnloadFont(font_);
    }
    if (has_sfx_) {
        UnloadSound(sfx_);
    }
}

void TextEntity::step() {
    if (visible_chars_ >= text_.size()) {
        return;
    }
    if (pause_timer_ > 0) {
        --pause_timer_;
        return;
    }
    if (text_[visible_chars_] == '^' && visible_chars_ + 1 < text_.size()) {
        char d = text_[visible_chars_ + 1];
        if (d >= '1' && d <= '9') {
            pause_timer_ = (d - '0') * 4;
            text_.erase(visible_chars_, 2);
            return;
        }
    }

    ++step_counter_;
    if (step_counter_ >= steps_per_char_) {
        step_counter_ = 0;
        char next_char = text_[visible_chars_];
        ++visible_chars_;

        if (has_sfx_ && next_char != ' ' && next_char != '\n' && next_char != '\t') {
            PlaySound(sfx_);
        }
    }
}

void TextEntity::draw() {
    if (visible_chars_ == 0) {
        return;
    }

    std::string visible = text_.substr(0, visible_chars_);
    if (bitmap_font_.is_loaded()) {
        bitmap_font_.draw_text(visible, x_, y_, font_size_ / 16.0f, hspace_, vspace_, centered_, false, color_);
        return;
    }

    DrawTextEx(font_, visible.c_str(), Vector2{x_, y_}, font_size_, 1.0f, color_);
}

bool TextEntity::is_finished() const {
    return visible_chars_ >= text_.size();
}

void TextEntity::skip() {
    visible_chars_ = text_.size();
}

void TextEntity::set_font_size(float font_size) {
    font_size_ = font_size;
}

void TextEntity::set_centered(bool centered) {
    centered_ = centered;
}

void TextEntity::set_spacing(float hspace, float vspace) {
    hspace_ = hspace;
    vspace_ = vspace;
}

void TextEntity::set_pos(float x, float y) {
    x_ = x;
    y_ = y;
}

void TextEntity::set_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    color_ = Color{r, g, b, a};
}

void TextEntity::set_text(const std::string& text) {
    text_ = text;
    visible_chars_ = text_.size();
}

void TextEntity::set_font(const std::string& font_path) {
    if (custom_font_) {
        UnloadFont(font_);
        custom_font_ = false;
    }
    bitmap_font_.unload();

    if (bitmap_font_.load(font_path)) {
        return;
    }

    if (!font_path.empty()) {
        font_ = LoadFontEx(font_path.c_str(), static_cast<int>(font_size_), nullptr, 0);
        custom_font_ = font_.texture.id != 0;
    }
    if (!custom_font_) {
        font_ = GetFontDefault();
    }
}
