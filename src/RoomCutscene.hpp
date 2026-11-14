#pragma once

#include "Room.hpp"

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <memory>
#include <string>

class ImageEntity;
class TextEntity;

class RoomCutscene : public Room {
public:
    RoomCutscene(const std::string& image_path,
                 float img_x,
                 float img_y,
                 float img_scale,
                 float pan_dx,
                 float pan_dy,
                 float fade_speed,
                 const std::string& text,
                 float text_x,
                 float text_y,
                 const std::string& font_path,
                 const std::string& sfx_path,
                 int hold_steps,
                 luabridge::LuaRef on_end,
                 luabridge::LuaRef on_skip);
    ~RoomCutscene() override;

    void step() override;

    std::shared_ptr<ImageEntity> image() const;
    std::shared_ptr<TextEntity> text() const;
    void set_debug(bool debug);

private:
    std::shared_ptr<ImageEntity> image_;
    std::shared_ptr<TextEntity> text_;
    int hold_steps_ = -1;
    int hold_timer_ = 0;
    luabridge::LuaRef on_end_;
    luabridge::LuaRef on_skip_;
    bool ending_ = false;
    bool skipped_ = false;
    bool ended_ = false;
    bool debug_ = false;
};
