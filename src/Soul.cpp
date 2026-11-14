#include "Soul.hpp"

#include <raylib.h>

Soul::Soul(float x, float y, const std::string& sprite_path, float scale)
    : SpriteEntity(sprite_path, x, y, scale) {}

Soul::~Soul() = default;

bool Soul::up_pressed() const {
    return IsKeyPressed(KEY_UP);
}

bool Soul::down_pressed() const {
    return IsKeyPressed(KEY_DOWN);
}

bool Soul::left_pressed() const {
    return IsKeyPressed(KEY_LEFT);
}

bool Soul::right_pressed() const {
    return IsKeyPressed(KEY_RIGHT);
}

bool Soul::up_held() const {
    return IsKeyDown(KEY_UP);
}

bool Soul::down_held() const {
    return IsKeyDown(KEY_DOWN);
}

bool Soul::left_held() const {
    return IsKeyDown(KEY_LEFT);
}

bool Soul::right_held() const {
    return IsKeyDown(KEY_RIGHT);
}
