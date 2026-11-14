#include "Room.hpp"
#include "Entity.hpp"

#include <raylib.h>
#include <utility>

Room::~Room() = default;

void Room::add_entity(std::shared_ptr<Entity> entity) {
    if (entity) {
        entities_.push_back(std::move(entity));
    }
}

void Room::step() {
    for (std::size_t i = 0; i < entities_.size(); ++i) {
        entities_[i]->step();
    }
}

void Room::draw() {
    ClearBackground(BLACK);
    for (std::size_t i = 0; i < entities_.size(); ++i) {
        entities_[i]->draw();
    }
}
