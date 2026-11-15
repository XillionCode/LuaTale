#pragma once

#include "Entity.hpp"

#include <raylib.h>

class BoxEntity : public Entity {
public:
    BoxEntity(float x = 0.0f,
              float y = 0.0f,
              float width = 100.0f,
              float height = 100.0f,
              float border_thickness = 6.0f);
    ~BoxEntity() override = default;

    void draw() override;

    void set_pos(float x, float y);
    void set_size(float width, float height);
    void set_border_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void set_fill_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

private:
    float x_ = 0.0f;
    float y_ = 0.0f;
    float width_ = 100.0f;
    float height_ = 100.0f;
    float border_thickness_ = 6.0f;
    Color border_color_ = WHITE;
    Color fill_color_ = BLACK;
};
