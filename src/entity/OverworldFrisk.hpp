#pragma once

#include "PlayerEntity.hpp"

#include <array>
#include <raylib.h>
#include <vector>

class OverworldFrisk : public PlayerEntity {
public:
    OverworldFrisk(float x = 0.0f, float y = 0.0f);
    ~OverworldFrisk() override;

    void step() override;
    void draw() override;

    float width() const override;
    float height() const override;

private:
    enum Facing { Down = 0, Right = 1, Up = 2, Left = 3 };

    void load_direction(Facing dir, const char* prefix, int count);

    std::array<std::vector<Texture2D>, 4> frames_;
    Facing facing_ = Down;
    float frame_accum_ = 0.0f;
    bool moving_ = false;
    bool dance_toggle_ = false;
};
