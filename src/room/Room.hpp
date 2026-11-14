#pragma once

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <memory>
#include <vector>

class Entity;

class Room : public std::enable_shared_from_this<Room> {
public:
    Room() = default;
    virtual ~Room();

    void add_entity(std::shared_ptr<Entity> entity);
    void on_delay(int frames, luabridge::LuaRef callback);

    virtual void step();
    virtual void draw();

protected:
    struct DelayTimer {
        int remaining_frames = 0;
        luabridge::LuaRef callback;
    };

    std::vector<std::shared_ptr<Entity>> entities_;
    std::vector<DelayTimer> timers_;
};
