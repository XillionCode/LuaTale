#include "MenuSoul.hpp"

MenuSoul::MenuSoul(float x, float y, bool vert_lock, bool hor_lock)
    : Soul(x, y),
      vert_lock_(vert_lock),
      hor_lock_(hor_lock) {}

MenuSoul::~MenuSoul() = default;

void MenuSoul::add_snap_point(float pos) {
    snap_points_.push_back(pos);
    if (snap_points_.size() == 1) {
        apply_snap();
    }
}

std::size_t MenuSoul::get_selected() const {
    return selected_;
}

void MenuSoul::apply_snap() {
    if (snap_points_.empty()) {
        return;
    }
    if (vert_lock_) {
        x_ = snap_points_[selected_];
    } else if (hor_lock_) {
        y_ = snap_points_[selected_];
    }
}

void MenuSoul::step() {
    if (snap_points_.empty()) {
        return;
    }

    const std::size_t count = snap_points_.size();
    if (vert_lock_) {
        if (left_pressed()) {
            selected_ = (selected_ == 0) ? (count - 1) : (selected_ - 1);
            apply_snap();
        } else if (right_pressed()) {
            selected_ = (selected_ + 1) % count;
            apply_snap();
        }
    } else if (hor_lock_) {
        if (up_pressed()) {
            selected_ = (selected_ == 0) ? (count - 1) : (selected_ - 1);
            apply_snap();
        } else if (down_pressed()) {
            selected_ = (selected_ + 1) % count;
            apply_snap();
        }
    }
}
