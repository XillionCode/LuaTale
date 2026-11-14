#pragma once

#include <memory>

class Entity : public std::enable_shared_from_this<Entity> {
public:
    Entity() = default;
    virtual ~Entity();

    virtual void step();
    virtual void draw();
};
