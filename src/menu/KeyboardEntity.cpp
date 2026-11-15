#include "KeyboardEntity.hpp"

#include <raylib.h>
#include <cstdint>
#include <iostream>
#include <utility>

KeyboardEntity::KeyboardEntity(float x,
                               float y,
                               const std::string& font_name,
                               luabridge::LuaRef on_update,
                               luabridge::LuaRef on_quit,
                               luabridge::LuaRef on_done) {
    if (on_update.state() != nullptr && on_update.isFunction()) {
        on_update_ = std::move(on_update);
    }
    if (on_quit.state() != nullptr && on_quit.isFunction()) {
        on_quit_ = std::move(on_quit);
    }
    if (on_done.state() != nullptr && on_done.isFunction()) {
        on_done_ = std::move(on_done);
    }
    font_.load(font_name);
    layout_.build(x, y);
}

KeyboardEntity::~KeyboardEntity() = default;

const std::string& KeyboardEntity::typed_text() const {
    return typed_text_;
}

void KeyboardEntity::set_typed_text(const std::string& text) {
    typed_text_ = text.substr(0, 6);
}

void KeyboardEntity::notify_update() {
    if (on_update_ && on_update_->isFunction()) {
        auto res = (*on_update_)(typed_text_);
        if (!res) {
            std::cerr << "KeyboardEntity on_update error: " << res.errorMessage() << '\n';
        }
    }
}

void KeyboardEntity::step() {
    if (layout_.rows().empty()) {
        return;
    }

    layout_.handle_navigation();

    if (IsKeyPressed(KEY_X) || IsKeyPressed(KEY_LEFT_SHIFT) || IsKeyPressed(KEY_RIGHT_SHIFT)) {
        if (!typed_text_.empty()) {
            typed_text_.pop_back();
            notify_update();
        }
        return;
    }

    if (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_ENTER)) {
        const KeyItem& item = layout_.current();
        if (!item.is_action) {
            if (typed_text_.size() < 6) {
                typed_text_ += item.label[0];
            } else {
                typed_text_.back() = item.label[0];
            }
            notify_update();
        } else if (item.action_id == 0 && on_quit_ && on_quit_->isFunction()) {
            auto res = (*on_quit_)();
            if (!res) {
                std::cerr << "KeyboardEntity on_quit error: " << res.errorMessage() << '\n';
            }
        } else if (item.action_id == 1 && !typed_text_.empty()) {
            typed_text_.pop_back();
            notify_update();
        } else if (item.action_id == 2 && on_done_ && on_done_->isFunction()) {
            auto res = (*on_done_)(typed_text_);
            if (!res) {
                std::cerr << "KeyboardEntity on_done error: " << res.errorMessage() << '\n';
            }
        }
    }
}

void KeyboardEntity::draw() {
    const auto& rows = layout_.rows();
    for (std::size_t r = 0; r < rows.size(); ++r) {
        for (std::size_t c = 0; c < rows[r].size(); ++c) {
            const KeyItem& item = rows[r][c];
            const Color color = (r == layout_.row() && c == layout_.col()) ? YELLOW : WHITE;
            if (font_.is_loaded()) {
                font_.draw_text(item.label, item.x, item.y, 2.0f, 0.0f, 0.0f, false, item.shakes, color);
            } else {
                DrawText(item.label.c_str(), static_cast<int32_t>(item.x), static_cast<int32_t>(item.y), 24, color);
            }
        }
    }
}
