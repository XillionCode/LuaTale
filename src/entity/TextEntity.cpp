#include "TextEntity.hpp"

#include <iostream>
#include <utility>

TextEntity::TextEntity(const std::string& text,
                       float x,
                       float y,
                       const std::string& font_path,
                       const std::string& sfx_path,
                       float font_size,
                       int steps_per_char)
    : x_(x),
      y_(y),
      font_size_(font_size),
      steps_per_char_(steps_per_char > 0 ? steps_per_char : 1) {
    set_font(font_path);

    if (!sfx_path.empty()) {
        sfx_ = LoadSound(sfx_path.c_str());
        has_sfx_ = sfx_.frameCount > 0;
    }

    apply_parsed(text, false);
}

TextEntity::~TextEntity() {
    if (custom_font_) {
        UnloadFont(font_);
    }
    if (has_sfx_) {
        UnloadSound(sfx_);
    }
}

void TextEntity::apply_parsed(const std::string& raw, bool reveal_all) {
    set_parsed_page(TextParser::parse_single(raw), reveal_all);
}

void TextEntity::set_parsed_page(const ParsedPage& page, bool reveal_all) {
    text_ = page.text;
    color_runs_ = page.color_runs;
    events_ = page.events;
    next_event_ = 0;
    step_counter_ = 0;
    pause_timer_ = 0;
    visible_chars_ = reveal_all ? text_.size() : 0;
    if (reveal_all) {
        for (const TextEvent& ev : events_) {
            if (ev.type == TextEvent::Type::Emote) {
                current_emote_ = ev.value;
            }
        }
        next_event_ = events_.size();
    }
}

void TextEntity::process_events_at_cursor() {
    while (next_event_ < events_.size() && events_[next_event_].char_index <= visible_chars_) {
        const TextEvent& ev = events_[next_event_++];
        if (ev.type == TextEvent::Type::Pause) {
            pause_timer_ = ev.value;
            break;
        }
        if (ev.type == TextEvent::Type::Emote) {
            current_emote_ = ev.value;
            if (on_emote_cpp_) {
                on_emote_cpp_(current_emote_);
            }
            if (on_emote_lua_ && on_emote_lua_->isFunction()) {
                auto res = (*on_emote_lua_)(current_emote_);
                if (!res) {
                    std::cerr << "TextEntity on_emote error: " << res.errorMessage() << '\n';
                }
            }
        }
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

    process_events_at_cursor();
    if (pause_timer_ > 0) {
        return;
    }

    ++step_counter_;
    if (step_counter_ >= steps_per_char_) {
        step_counter_ = 0;
        char next_char = text_[visible_chars_];
        ++visible_chars_;

        if (has_sfx_ && next_char != ' ' && next_char != '\n' && next_char != '\t') {
            PlaySound(sfx_);
        }
        process_events_at_cursor();
    }
}

void TextEntity::draw() {
    if (visible_chars_ == 0) {
        return;
    }

    std::string visible = text_.substr(0, visible_chars_);
    if (bitmap_font_.is_loaded()) {
        bitmap_font_.draw_text(
            visible, x_, y_, font_size_ / 16.0f, hspace_, vspace_, centered_, false, color_, rotation_, &color_runs_);
        return;
    }

    DrawTextEx(font_, visible.c_str(), Vector2{x_, y_}, font_size_, 1.0f, color_);
}

bool TextEntity::is_finished() const {
    return visible_chars_ >= text_.size();
}

bool TextEntity::is_typing() const {
    return visible_chars_ < text_.size() && pause_timer_ == 0;
}

int TextEntity::current_emote() const {
    return current_emote_;
}

void TextEntity::skip() {
    visible_chars_ = text_.size();
    pause_timer_ = 0;
    while (next_event_ < events_.size()) {
        const TextEvent& ev = events_[next_event_++];
        if (ev.type == TextEvent::Type::Emote) {
            current_emote_ = ev.value;
            if (on_emote_cpp_) {
                on_emote_cpp_(current_emote_);
            }
            if (on_emote_lua_ && on_emote_lua_->isFunction()) {
                auto res = (*on_emote_lua_)(current_emote_);
                if (!res) {
                    std::cerr << "TextEntity on_emote error: " << res.errorMessage() << '\n';
                }
            }
        }
    }
}

void TextEntity::restart(const std::string& text) {
    apply_parsed(text, false);
}

void TextEntity::on_emote(luabridge::LuaRef callback) {
    if (callback.isFunction()) {
        on_emote_lua_ = std::move(callback);
    } else {
        on_emote_lua_.reset();
    }
}

void TextEntity::set_cpp_emote_callback(std::function<void(int)> callback) {
    on_emote_cpp_ = std::move(callback);
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

void TextEntity::set_rotation(float rotation) {
    rotation_ = rotation;
}

void TextEntity::set_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    color_ = Color{r, g, b, a};
}

void TextEntity::set_text(const std::string& text) {
    apply_parsed(text, true);
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
