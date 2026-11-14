#include "Room.hpp"
#include "entity/Entity.hpp"

#include <iostream>
#include <raylib.h>
#include <utility>

Room::~Room() = default;

void Room::add_entity(std::shared_ptr<Entity> entity) {
    if (entity) {
        entities_.push_back(std::move(entity));
    }
}

void Room::on_delay(int frames, luabridge::LuaRef callback) {
    if (callback.isFunction()) {
        timers_.push_back({frames, std::move(callback)});
    }
}

void Room::step() {
    auto self = shared_from_this();

    std::vector<luabridge::LuaRef> ready;
    for (auto it = timers_.begin(); it != timers_.end();) {
        if (--it->remaining_frames <= 0) {
            ready.push_back(std::move(it->callback));
            it = timers_.erase(it);
        } else {
            ++it;
        }
    }
    for (auto& cb : ready) {
        auto res = cb();
        if (!res) {
            std::cerr << "Room on_delay error: " << res.errorMessage() << '\n';
        }
    }

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
