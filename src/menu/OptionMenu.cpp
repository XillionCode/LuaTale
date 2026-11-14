#include "OptionMenu.hpp"
#include "entity/TextEntity.hpp"

#include <raylib.h>
#include <iostream>
#include <utility>

OptionMenu::OptionMenu(float center_x,
                       float top_y,
                       float spread_x,
                       float spread_y,
                       const std::string& font_path)
    : center_x_(center_x),
      top_y_(top_y),
      spread_x_(spread_x),
      spread_y_(spread_y),
      font_path_(font_path) {}

OptionMenu::~OptionMenu() = default;

void OptionMenu::add_option(const std::string& label, luabridge::LuaRef on_select) {
    auto text = std::make_shared<TextEntity>(label, center_x_, top_y_, font_path_, "", 32.0f, 1);
    text->set_centered(true);
    text->skip();
    add_child(text);

    labels_.push_back(std::move(text));
    callbacks_.push_back(std::move(on_select));

    relayout();
    update_colors();
}

void OptionMenu::relayout() {
    const std::size_t count = labels_.size();
    if (count == 1) {
        labels_[0]->set_pos(center_x_, top_y_);
    } else if (count == 2) {
        labels_[0]->set_pos(center_x_ - spread_x_, top_y_);
        labels_[1]->set_pos(center_x_ + spread_x_, top_y_);
    } else if (count == 3) {
        labels_[0]->set_pos(center_x_ - spread_x_, top_y_);
        labels_[1]->set_pos(center_x_ + spread_x_, top_y_);
        labels_[2]->set_pos(center_x_, top_y_ + spread_y_);
    } else if (count >= 4) {
        labels_[0]->set_pos(center_x_, top_y_);
        labels_[1]->set_pos(center_x_ - spread_x_, top_y_ + spread_y_);
        labels_[2]->set_pos(center_x_ + spread_x_, top_y_ + spread_y_);
        labels_[3]->set_pos(center_x_, top_y_ + spread_y_ * 2.0f);
    }
}

void OptionMenu::update_colors() {
    for (std::size_t i = 0; i < labels_.size(); ++i) {
        if (i == selected_) {
            labels_[i]->set_color(255, 255, 0, 255);
        } else {
            labels_[i]->set_color(255, 255, 255, 255);
        }
    }
}

void OptionMenu::self_step() {
    const std::size_t count = labels_.size();
    if (count == 0) {
        return;
    }

    std::size_t next = selected_;
    if (count == 3) {
        if (IsKeyPressed(KEY_LEFT)) {
            next = (selected_ == 1) ? 0 : (selected_ == 0 ? 1 : 0);
        } else if (IsKeyPressed(KEY_RIGHT)) {
            next = (selected_ == 0) ? 1 : (selected_ == 1 ? 0 : 1);
        } else if (IsKeyPressed(KEY_DOWN)) {
            next = 2;
        } else if (IsKeyPressed(KEY_UP) && selected_ == 2) {
            next = 0;
        }
    } else if (count >= 4) {
        if (IsKeyPressed(KEY_UP)) {
            next = 0;
        } else if (IsKeyPressed(KEY_LEFT)) {
            next = 1;
        } else if (IsKeyPressed(KEY_RIGHT)) {
            next = 2;
        } else if (IsKeyPressed(KEY_DOWN)) {
            next = 3;
        }
    } else {
        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_UP)) {
            next = (selected_ == 0) ? (count - 1) : (selected_ - 1);
        } else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_DOWN)) {
            next = (selected_ + 1) % count;
        }
    }

    if (next != selected_) {
        selected_ = next;
        update_colors();
    }

    if (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_ENTER)) {
        if (selected_ < callbacks_.size() && callbacks_[selected_].isFunction()) {
            auto res = callbacks_[selected_]();
            if (!res) {
                std::cerr << "OptionMenu on_select error: " << res.errorMessage() << '\n';
            }
        }
    }
}
