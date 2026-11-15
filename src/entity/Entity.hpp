#pragma once

#include <memory>

class Entity : public std::enable_shared_from_this<Entity> {
public:
    Entity() = default;
    virtual ~Entity();

    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

    virtual void step();
    virtual void draw();

    void set_visible(bool visible);
    bool visible() const;

    void set_active(bool active);
    bool active() const;

    virtual float depth() const;
    void set_depth(float depth);
    bool has_custom_depth() const;

private:
    bool visible_ = true;
    bool active_ = true;
    float depth_ = 0.0f;
    bool custom_depth_ = false;
};
