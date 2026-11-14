#include "FadeOverlayEntity.hpp"

#include <algorithm>

FadeOverlayEntity::FadeOverlayEntity(unsigned char r, unsigned char g, unsigned char b, float speed)
    : color_{r, g, b, 0}, speed_(speed) {}

void FadeOverlayEntity::step() {
    alpha_ = std::clamp(alpha_ + speed_, 0.0f, 1.0f);
    color_.a = static_cast<unsigned char>(alpha_ * 255.0f);
}

void FadeOverlayEntity::draw() {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), color_);
}
