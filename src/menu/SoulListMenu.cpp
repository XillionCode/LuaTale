#include "SoulListMenu.hpp"
#include "entity/MenuSoul.hpp"
#include "entity/TextEntity.hpp"

#include <raylib.h>
#include <iostream>
#include <utility>

SoulListMenu::SoulListMenu(float x,
                           float y,
                           float spacing,
                           float soul_offset_x,
                           const std::string& font_path,
                           bool not_soul,
                           bool horizontal)
    : x_(x),
      y_(y),
      spacing_(spacing),
      soul_offset_x_(soul_offset_x),
      font_path_(font_path),
      not_soul_(not_soul),
      horizontal_(horizontal) {
    if (!not_soul_) {
        soul_ = std::make_shared<MenuSoul>(x_ + soul_offset_x_, y_, horizontal_, !horizontal_);
        add_child(soul_);
    }
}

SoulListMenu::~SoulListMenu() = default;

void SoulListMenu::update_colors() {
    if (!not_soul_) {
        return;
    }
    for (std::size_t i = 0; i < labels_.size(); ++i) {
        labels_[i]->set_color(255, 255, i == selected_ ? 0 : 255, 255);
    }
}

void SoulListMenu::add_option(const std::string& label, luabridge::LuaRef on_select) {
    const float idx = static_cast<float>(callbacks_.size());
    const float opt_x = horizontal_ ? (x_ + idx * spacing_) : x_;
    const float opt_y = horizontal_ ? y_ : (y_ + idx * spacing_);

    auto text = std::make_shared<TextEntity>(label, opt_x, opt_y, font_path_, "", 32.0f, 1);
    text->skip();
    add_child(text);
    labels_.push_back(text);

    if (!not_soul_) {
        soul_->add_snap_point(horizontal_ ? (opt_x + soul_offset_x_) : (opt_y + 4.0f));
    } else {
        update_colors();
    }
    callbacks_.push_back(std::move(on_select));
}

void SoulListMenu::clear_options() {
    clear_children();
    labels_.clear();
    callbacks_.clear();
    selected_ = 0;
    if (!not_soul_ && soul_) {
        soul_->clear_snap_points();
        add_child(soul_);
    }
}

void SoulListMenu::set_option_label(std::size_t index, const std::string& label) {
    if (index < labels_.size()) {
        labels_[index]->set_text(label);
    }
}

void SoulListMenu::set_option_color(std::size_t index,
                                    unsigned char r,
                                    unsigned char g,
                                    unsigned char b,
                                    unsigned char a) {
    if (index < labels_.size()) {
        labels_[index]->set_color(r, g, b, a);
    }
}

void SoulListMenu::set_selected(std::size_t index) {
    selected_ = index < callbacks_.size() ? index : 0;
    if (!not_soul_ && soul_) {
        soul_->set_selected(selected_);
    } else {
        update_colors();
    }
}

std::size_t SoulListMenu::selected() const {
    return not_soul_ ? selected_ : (soul_ ? soul_->get_selected() : 0);
}

void SoulListMenu::on_move(luabridge::LuaRef callback) {
    on_move_ = std::move(callback);
}

void SoulListMenu::set_font(const std::string& font_path) {
    font_path_ = font_path;
    for (const auto& label : labels_) {
        label->set_font(font_path_);
    }
}

void SoulListMenu::self_step() {
    if (callbacks_.empty()) {
        return;
    }

    const std::size_t prev_selected = selected();
    if (not_soul_) {
        const std::size_t count = callbacks_.size();
        const bool prev = horizontal_ ? IsKeyPressed(KEY_LEFT) : IsKeyPressed(KEY_UP);
        const bool next = horizontal_ ? IsKeyPressed(KEY_RIGHT) : IsKeyPressed(KEY_DOWN);
        if (prev) {
            selected_ = (selected_ == 0) ? (count - 1) : (selected_ - 1);
            update_colors();
        } else if (next) {
            selected_ = (selected_ + 1) % count;
            update_colors();
        }
    } else if (soul_) {
        // MenuSoul steps after self_step(); check on_move after or compare?
        // Actually EntityGroup calls self_step() BEFORE children! So check key press or track selected_.
        const std::size_t count = callbacks_.size();
        const bool prev = horizontal_ ? IsKeyPressed(KEY_LEFT) : IsKeyPressed(KEY_UP);
        const bool next = horizontal_ ? IsKeyPressed(KEY_RIGHT) : IsKeyPressed(KEY_DOWN);
        if (count > 1 && (prev || next) && on_move_.isFunction()) {
            auto res = on_move_();
            if (!res) {
                std::cerr << "SoulListMenu on_move error: " << res.errorMessage() << '\n';
            }
        }
    }
    if (not_soul_ && selected_ != prev_selected && on_move_.isFunction()) {
        auto res = on_move_();
        if (!res) {
            std::cerr << "SoulListMenu on_move error: " << res.errorMessage() << '\n';
        }
    }

    if (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_ENTER)) {
        std::size_t idx = not_soul_ ? selected_ : soul_->get_selected();
        if (idx < callbacks_.size() && callbacks_[idx].isFunction()) {
            auto res = callbacks_[idx]();
            if (!res) {
                std::cerr << "SoulListMenu on_select error: " << res.errorMessage() << '\n';
            }
        }
    }
}
