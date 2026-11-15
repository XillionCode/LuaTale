#pragma once

#include "SpriteEntity.hpp"

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <cstdint>
#include <optional>
#include <raylib.h>
#include <string>
#include <vector>

class NPCEntity : public SpriteEntity {
public:
    NPCEntity(const std::string& sprite_path = "", float x = 0.0f, float y = 0.0f, float scale = 1.0f);
    ~NPCEntity() override;

    void step() override;
    void draw() override;

    void add_frame(const std::string& path);
    void set_frame(int32_t index);
    void set_anim_speed(float frames_per_step);

    void set_solid(bool solid);
    bool solid() const;
    Rectangle hitbox() const;

    void on_interact(luabridge::LuaRef callback);
    void trigger_interact();

    void walk_to(float target_x, float target_y, float speed, luabridge::LuaRef on_arrive);

private:
    std::vector<Texture2D> frames_;
    int32_t current_frame_ = 0;
    float frame_accum_ = 0.0f;
    float anim_speed_ = 0.0f;
    bool solid_ = false;

    bool walking_ = false;
    float walk_target_x_ = 0.0f;
    float walk_target_y_ = 0.0f;
    float walk_speed_ = 2.0f;
    std::optional<luabridge::LuaRef> on_arrive_;
    std::optional<luabridge::LuaRef> on_interact_;
};
