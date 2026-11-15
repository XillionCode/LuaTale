#pragma once

#include "BackgroundAtlas.hpp"
#include "Room.hpp"

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <memory>
#include <optional>
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>

class PlayerEntity;

struct RoomBackground {
    bool foreground = false;
    std::string definition;
    float x = 0.0f;
    float y = 0.0f;
    bool tile_x = false;
    bool tile_y = false;
};

struct RoomTile {
    float x = 0.0f;
    float y = 0.0f;
    std::string background;
    float source_x = 0.0f;
    float source_y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
    int depth = 0;
    float scale_x = 1.0f;
    float scale_y = 1.0f;
    Color tint = WHITE;
};

struct SlopeWall {
    enum Type { SUL, SUR, SDL, SDR };
    Type type = SUL;
    Rectangle rect{};
};

struct RoomDoor {
    std::string name;
    Rectangle rect{};
};

class OverworldRoom : public Room {
public:
    explicit OverworldRoom(const std::string& json_path);
    ~OverworldRoom() override;

    void step() override;
    void draw() override;

    void set_follow(std::shared_ptr<PlayerEntity> target);
    void on_door(luabridge::LuaRef callback);
    void add_ui_entity(std::shared_ptr<Entity> entity);
    void clear_ui_entities();
    bool collides(Rectangle hitbox) const;
    float marker_x(const std::string& name) const;
    float marker_y(const std::string& name) const;
    float spawn_x() const;
    float spawn_y() const;
    float view_x() const;
    float view_y() const;
    float width() const;
    float height() const;

private:
    bool load_room(const std::string& json_path);
    void draw_background_layer(const RoomBackground& layer);

    BackgroundAtlas atlas_;
    std::vector<RoomBackground> backgrounds_;
    std::vector<RoomTile> tiles_;
    std::vector<Rectangle> solid_walls_;
    std::vector<SlopeWall> slope_walls_;
    std::vector<RoomDoor> doors_;
    std::vector<std::shared_ptr<Entity>> ui_entities_;
    std::unordered_map<std::string, Vector2> markers_;
    std::weak_ptr<PlayerEntity> follow_target_;
    std::optional<luabridge::LuaRef> on_door_;
    bool door_armed_ = false;
    bool door_triggered_ = false;
    float room_width_ = 320.0f;
    float room_height_ = 240.0f;
    float spawn_x_ = 0.0f;
    float spawn_y_ = 0.0f;
    Camera2D camera_{};
};
