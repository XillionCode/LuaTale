#pragma once

#include "Room.hpp"

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <memory>
#include <string>

class OptionMenu;

class MenuRoom : public Room {
public:
    MenuRoom(float center_x = 320.0f,
             float top_y = 260.0f,
             float spread_x = 120.0f,
             float spread_y = 44.0f,
             const std::string& font_path = "fnt_maintext");
    ~MenuRoom() override;

    void add_option(const std::string& label, luabridge::LuaRef on_select);
    std::shared_ptr<OptionMenu> menu() const;

private:
    std::shared_ptr<OptionMenu> menu_;
};
