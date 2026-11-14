#include "BattleSoul.hpp"

BattleSoul::BattleSoul(float x, float y) : PlayerEntity(x, y, 4.0f) {
    texture_ = LoadTexture("assets/Sprites/spr_heart_0.png");
    set_hitbox(4.0f, 4.0f, 8.0f, 8.0f);
}

BattleSoul::~BattleSoul() {
    if (texture_.id > 0) {
        UnloadTexture(texture_);
    }
}

float BattleSoul::width() const {
    return texture_.id > 0 ? static_cast<float>(texture_.width) : 16.0f;
}

float BattleSoul::height() const {
    return texture_.id > 0 ? static_cast<float>(texture_.height) : 16.0f;
}

void BattleSoul::step() {
    const float speed = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT) || IsKeyDown(KEY_X))
                            ? (speed_ * 0.5f)
                            : speed_;
    const float dx = (IsKeyDown(KEY_RIGHT) ? 1.0f : 0.0f) - (IsKeyDown(KEY_LEFT) ? 1.0f : 0.0f);
    const float dy = (IsKeyDown(KEY_DOWN) ? 1.0f : 0.0f) - (IsKeyDown(KEY_UP) ? 1.0f : 0.0f);

    x_ += dx * speed;
    y_ += dy * speed;
    clamp_to_bounds();
}

void BattleSoul::draw() {
    if (texture_.id > 0) {
        DrawTextureV(texture_, Vector2{x_, y_}, WHITE);
    }
}
