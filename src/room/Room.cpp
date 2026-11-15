#include "Room.hpp"
#include "entity/Entity.hpp"

#include <cctype>
#include <iostream>
#include <raylib.h>
#include <utility>

namespace {

bool is_room_key_pressed(const std::string& key) {
    if (key == "z" || key == "Z") {
        return IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_ENTER);
    }
    if (key == "x" || key == "X") {
        return IsKeyPressed(KEY_X) || IsKeyPressed(KEY_LEFT_SHIFT) || IsKeyPressed(KEY_RIGHT_SHIFT);
    }
    if (key == "c" || key == "C") {
        return IsKeyPressed(KEY_C) || IsKeyPressed(KEY_LEFT_CONTROL) || IsKeyPressed(KEY_RIGHT_CONTROL);
    }
    if (key == "up") {
        return IsKeyPressed(KEY_UP);
    }
    if (key == "down") {
        return IsKeyPressed(KEY_DOWN);
    }
    if (key == "left") {
        return IsKeyPressed(KEY_LEFT);
    }
    if (key == "right") {
        return IsKeyPressed(KEY_RIGHT);
    }
    if (key.size() == 1) {
        const unsigned char ch = static_cast<unsigned char>(std::tolower(static_cast<unsigned char>(key[0])));
        if (ch >= 'a' && ch <= 'z') {
            return IsKeyPressed(KEY_A + (ch - 'a'));
        }
        if (ch >= '0' && ch <= '9') {
            return IsKeyPressed(KEY_ZERO + (ch - '0'));
        }
    }
    return false;
}

}  // namespace

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

void Room::set_key_event(const std::string& key, luabridge::LuaRef callback) {
    for (auto it = key_events_.begin(); it != key_events_.end(); ++it) {
        if (it->key == key) {
            if (callback.isFunction()) {
                it->callback = std::move(callback);
            } else {
                key_events_.erase(it);
            }
            return;
        }
    }
    if (callback.isFunction()) {
        key_events_.push_back({key, std::move(callback)});
    }
}

void Room::step() {
    auto self = shared_from_this();

    std::vector<KeyEvent> events = key_events_;
    for (auto& ev : events) {
        if (is_room_key_pressed(ev.key)) {
            auto res = ev.callback();
            if (!res) {
                std::cerr << "Room key_event error (" << ev.key << "): " << res.errorMessage() << '\n';
            }
        }
    }

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
        if (entities_[i]->active()) {
            entities_[i]->step();
        }
    }
}

void Room::draw() {
    ClearBackground(BLACK);
    for (std::size_t i = 0; i < entities_.size(); ++i) {
        if (entities_[i]->visible()) {
            entities_[i]->draw();
        }
    }
}
