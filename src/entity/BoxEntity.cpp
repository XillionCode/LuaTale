#include "BoxEntity.hpp"

BoxEntity::BoxEntity(float x, float y, float width, float height, float border_thickness)
    : x_(x),
      y_(y),
      width_(width),
      height_(height),
      border_thickness_(border_thickness) {}

void BoxEntity::draw() {
    DrawRectangle(static_cast<int>(x_),
                  static_cast<int>(y_),
                  static_cast<int>(width_),
                  static_cast<int>(height_),
                  border_color_);
    const float inset = border_thickness_;
    if (width_ > inset * 2.0f && height_ > inset * 2.0f) {
        DrawRectangle(static_cast<int>(x_ + inset),
                      static_cast<int>(y_ + inset),
                      static_cast<int>(width_ - inset * 2.0f),
                      static_cast<int>(height_ - inset * 2.0f),
                      fill_color_);
    }
}

void BoxEntity::set_pos(float x, float y) {
    x_ = x;
    y_ = y;
}

void BoxEntity::set_size(float width, float height) {
    width_ = width;
    height_ = height;
}

void BoxEntity::set_border_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    border_color_ = Color{r, g, b, a};
}

void BoxEntity::set_fill_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    fill_color_ = Color{r, g, b, a};
}
