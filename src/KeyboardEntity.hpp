#pragma once

#include "BitmapFont.hpp"
#include "Entity.hpp"
#include "KeyboardLayout.hpp"

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <string>

class KeyboardEntity : public Entity {
public:
    KeyboardEntity(float x = 110.0f,
                   float y = 160.0f,
                   const std::string& font_name = "fnt_maintext",
                   luabridge::LuaRef on_update = luabridge::LuaRef(nullptr),
                   luabridge::LuaRef on_quit = luabridge::LuaRef(nullptr),
                   luabridge::LuaRef on_done = luabridge::LuaRef(nullptr));
    ~KeyboardEntity() override;

    void step() override;
    void draw() override;

    const std::string& typed_text() const;
    void set_typed_text(const std::string& text);

private:
    void notify_update();

    std::string typed_text_;
    luabridge::LuaRef on_update_;
    luabridge::LuaRef on_quit_;
    luabridge::LuaRef on_done_;

    KeyboardLayout layout_;
    BitmapFont font_;
};
