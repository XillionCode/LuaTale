#pragma once

#include "font/BitmapFont.hpp"
#include "Entity.hpp"

#include <raylib.h>
#include <cstddef>
#include <string>

class TextEntity : public Entity {
public:
    TextEntity(const std::string& text,
               float x,
               float y,
               const std::string& font_path = "",
               const std::string& sfx_path = "",
               float font_size = 32.0f,
               int steps_per_char = 2);
    ~TextEntity() override;

    void step() override;
    void draw() override;

    bool is_finished() const;
    void skip();
    void set_font_size(float font_size);
    void set_font(const std::string& font_path);
    void set_centered(bool centered);
    void set_spacing(float hspace, float vspace);
    void set_pos(float x, float y);
    void set_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void set_text(const std::string& text);

private:
    std::string text_;
    float x_ = 0.0f;
    float y_ = 0.0f;
    float font_size_ = 32.0f;
    int steps_per_char_ = 2;
    int step_counter_ = 0;
    int pause_timer_ = 0;
    std::size_t visible_chars_ = 0;

    Font font_{};
    bool custom_font_ = false;
    BitmapFont bitmap_font_;
    float hspace_ = 0.0f;
    float vspace_ = 0.0f;
    bool centered_ = false;
    Color color_ = WHITE;
    Sound sfx_{};
    bool has_sfx_ = false;
};
