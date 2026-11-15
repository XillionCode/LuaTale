#include "Entity.hpp"

Entity::~Entity() = default;

void Entity::step() {}

void Entity::draw() {}

void Entity::set_visible(bool visible) {
    visible_ = visible;
}

bool Entity::visible() const {
    return visible_;
}

void Entity::set_active(bool active) {
    active_ = active;
}

bool Entity::active() const {
    return active_;
}
