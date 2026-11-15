#pragma once

#include "Entity.hpp"

#include <functional>
#include <raylib.h>

class PlayerEntity : public Entity {
public:
    PlayerEntity(float x = 0.0f, float y = 0.0f, float speed = 3.0f);
    ~PlayerEntity() override = default;

    void set_pos(float x, float y);
    void set_bounds(float max_w, float max_h);
    void set_collider(std::function<bool(Rectangle)> collider);
    void set_debug(bool debug);

    float x() const;
    float y() const;
    virtual float width() const = 0;
    virtual float height() const = 0;

    Rectangle hitbox() const;
    Rectangle attempted_hitbox() const;

protected:
    void set_hitbox(float offset_x, float offset_y, float w, float h);
    void clamp_to_bounds();
    bool collides_at(float x, float y) const;

    float x_ = 0.0f;
    float y_ = 0.0f;
    float attempted_x_ = 0.0f;
    float attempted_y_ = 0.0f;
    float speed_ = 3.0f;

    float hb_x_ = 0.0f;
    float hb_y_ = 0.0f;
    float hb_w_ = 0.0f;
    float hb_h_ = 0.0f;

    bool has_bounds_ = false;
    bool debug_ = false;
    float bound_w_ = 0.0f;
    float bound_h_ = 0.0f;
    std::function<bool(Rectangle)> collider_;
};
