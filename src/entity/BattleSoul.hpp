#pragma once

#include "PlayerEntity.hpp"

#include <raylib.h>

class BattleSoul : public PlayerEntity {
public:
    BattleSoul(float x = 0.0f, float y = 0.0f);
    ~BattleSoul() override;

    void step() override;
    void draw() override;

    float width() const override;
    float height() const override;

private:
    Texture2D texture_{};
};
