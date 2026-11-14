#pragma once

#include "BackgroundAtlas.hpp"
#include "Room.hpp"

#include <raylib.h>
#include <string>
#include <vector>

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

class OverworldRoom : public Room {
public:
    explicit OverworldRoom(const std::string& json_path);
    ~OverworldRoom() override;

    void step() override;
    void draw() override;

private:
    bool load_room(const std::string& json_path);
    void draw_background_layer(const RoomBackground& layer);

    BackgroundAtlas atlas_;
    std::vector<RoomBackground> backgrounds_;
    std::vector<RoomTile> tiles_;
    float room_width_ = 320.0f;
    float room_height_ = 240.0f;
    Camera2D camera_{};
};
