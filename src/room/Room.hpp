#pragma once

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class Entity;

class Room : public std::enable_shared_from_this<Room> {
public:
    Room() = default;
    virtual ~Room();

    void add_entity(std::shared_ptr<Entity> entity);
    void on_delay(int32_t frames, luabridge::LuaRef callback);
    void set_key_event(const std::string& key, luabridge::LuaRef callback);

    virtual void step();
    virtual void draw();

protected:
    struct DelayTimer {
        int32_t remaining_frames = 0;
        luabridge::LuaRef callback;
    };

    struct KeyEvent {
        std::string key;
        luabridge::LuaRef callback;
    };

    std::vector<std::shared_ptr<Entity>> entities_;
    std::vector<DelayTimer> timers_;
    std::vector<KeyEvent> key_events_;
};
