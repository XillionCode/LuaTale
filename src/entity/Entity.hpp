#pragma once

#include <memory>

class Entity : public std::enable_shared_from_this<Entity> {
public:
    Entity() = default;
    virtual ~Entity();

    virtual void step();
    virtual void draw();

    void set_visible(bool visible);
    bool visible() const;

    void set_active(bool active);
    bool active() const;

private:
    bool visible_ = true;
    bool active_ = true;
};
