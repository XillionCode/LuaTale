#include "PlayerEntity.hpp"

#include <algorithm>

PlayerEntity::PlayerEntity(float x, float y, float speed)
    : x_(x), y_(y), attempted_x_(x), attempted_y_(y), speed_(speed) {}

void PlayerEntity::set_pos(float x, float y) {
    x_ = x;
    y_ = y;
    attempted_x_ = x;
    attempted_y_ = y;
}

void PlayerEntity::set_bounds(float max_w, float max_h) {
    has_bounds_ = true;
    bound_w_ = max_w;
    bound_h_ = max_h;
    clamp_to_bounds();
}

void PlayerEntity::set_collider(std::function<bool(Rectangle)> collider) {
    collider_ = std::move(collider);
}

void PlayerEntity::set_debug(bool debug) {
    debug_ = debug;
}

bool PlayerEntity::collides_at(float x, float y) const {
    if (!collider_) {
        return false;
    }
    return collider_(Rectangle{x + hb_x_, y + hb_y_, hb_w_, hb_h_});
}

float PlayerEntity::x() const {
    return x_;
}

float PlayerEntity::y() const {
    return y_;
}

Rectangle PlayerEntity::hitbox() const {
    return Rectangle{x_ + hb_x_, y_ + hb_y_, hb_w_, hb_h_};
}

Rectangle PlayerEntity::attempted_hitbox() const {
    return Rectangle{attempted_x_ + hb_x_, attempted_y_ + hb_y_, hb_w_, hb_h_};
}

void PlayerEntity::set_hitbox(float offset_x, float offset_y, float w, float h) {
    hb_x_ = offset_x;
    hb_y_ = offset_y;
    hb_w_ = w;
    hb_h_ = h;
}

void PlayerEntity::clamp_to_bounds() {
    if (!has_bounds_) {
        return;
    }
    const float max_x = std::max(0.0f, bound_w_ - width());
    const float max_y = std::max(0.0f, bound_h_ - height());
    x_ = std::clamp(x_, 0.0f, max_x);
    y_ = std::clamp(y_, 0.0f, max_y);
}
