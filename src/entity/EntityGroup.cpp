#include "EntityGroup.hpp"

#include <utility>

EntityGroup::~EntityGroup() = default;

void EntityGroup::add_child(std::shared_ptr<Entity> child) {
    if (child) {
        children_.push_back(std::move(child));
    }
}

void EntityGroup::clear_children() {
    children_.clear();
}

void EntityGroup::self_step() {}

void EntityGroup::self_draw() {}

void EntityGroup::step() {
    if (!active()) {
        return;
    }
    self_step();
    for (std::size_t i = 0; i < children_.size(); ++i) {
        if (children_[i]->active()) {
            children_[i]->step();
        }
    }
}

void EntityGroup::draw() {
    if (!visible()) {
        return;
    }
    self_draw();
    for (std::size_t i = 0; i < children_.size(); ++i) {
        if (children_[i]->visible()) {
            children_[i]->draw();
        }
    }
}
