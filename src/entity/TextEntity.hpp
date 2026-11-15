#pragma once

#include "Entity.hpp"
#include "font/BitmapFont.hpp"
#include "text/Parser.hpp"

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <cstddef>
#include <functional>
#include <optional>
#include <raylib.h>
#include <string>
#include <vector>

class TextEntity : public Entity {
public:
    TextEntity(const std::string& text,
               float x,
               float y,
               const std::string& font_path = "",
               const std::string& sfx_path = "",
               float font_size = 32.0f,
               int steps_per_char = 1);
    ~TextEntity() override;

    void step() override;
    void draw() override;

    bool is_finished() const;
    bool is_typing() const;
    int current_emote() const;
    void skip();
    void restart(const std::string& text);
    void set_parsed_page(const ParsedPage& page, bool reveal_all = false);
    void on_emote(luabridge::LuaRef callback);
    void set_cpp_emote_callback(std::function<void(int)> callback);

    void set_font_size(float font_size);
    void set_font(const std::string& font_path);
    void set_centered(bool centered);
    void set_spacing(float hspace, float vspace);
    void set_pos(float x, float y);
    void set_rotation(float rotation);
    void set_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void set_text(const std::string& text);

private:
    void apply_parsed(const std::string& raw, bool reveal_all);
    void process_events_at_cursor();

    std::string text_;
    std::vector<TextColorRun> color_runs_;
    std::vector<TextEvent> events_;
    std::size_t next_event_ = 0;
    int current_emote_ = 0;

    float x_ = 0.0f;
    float y_ = 0.0f;
    float rotation_ = 0.0f;
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
    std::optional<luabridge::LuaRef> on_emote_lua_;
    std::function<void(int)> on_emote_cpp_;
};
