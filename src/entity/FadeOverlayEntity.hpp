#pragma once

#include "Entity.hpp"

#include <raylib.h>

class FadeOverlayEntity : public Entity {
public:
    FadeOverlayEntity(unsigned char r, unsigned char g, unsigned char b, float speed);
    ~FadeOverlayEntity() override = default;

    void step() override;
    void draw() override;

private:
    Color color_{255, 255, 255, 0};
    float speed_ = 0.006f;
    float alpha_ = 0.0f;
};
