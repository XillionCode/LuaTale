#include "OverworldFrisk.hpp"

#include <cmath>
#include <string>

OverworldFrisk::OverworldFrisk(float x, float y) : PlayerEntity(x, y, 3.0f) {
    load_direction(Down, "assets/Sprites/spr_maincharad_", 4);
    load_direction(Right, "assets/Sprites/spr_maincharar_", 2);
    load_direction(Up, "assets/Sprites/spr_maincharau_", 4);
    load_direction(Left, "assets/Sprites/spr_maincharal_", 2);

    const float w = width();
    const float h = height();
    set_hitbox(2.0f, h * 0.5f, w - 4.0f, h * 0.5f);
}

OverworldFrisk::~OverworldFrisk() {
    for (auto& dir : frames_) {
        for (Texture2D& tex : dir) {
            if (tex.id > 0) {
                UnloadTexture(tex);
            }
        }
    }
}

void OverworldFrisk::load_direction(Facing dir, const char* prefix, int count) {
    frames_[dir].reserve(count);
    for (int i = 0; i < count; ++i) {
        std::string path = std::string(prefix) + std::to_string(i) + ".png";
        frames_[dir].push_back(LoadTexture(path.c_str()));
    }
}

float OverworldFrisk::width() const {
    return frames_[Down].empty() ? 20.0f : static_cast<float>(frames_[Down][0].width);
}

float OverworldFrisk::height() const {
    return frames_[Down].empty() ? 30.0f : static_cast<float>(frames_[Down][0].height);
}

void OverworldFrisk::step() {
    const bool left = IsKeyDown(KEY_LEFT);
    const bool right = IsKeyDown(KEY_RIGHT);
    const bool up = IsKeyDown(KEY_UP);
    const bool down = IsKeyDown(KEY_DOWN);

    float dx = (right ? 1.0f : 0.0f) - (left ? 1.0f : 0.0f);
    float dy = up ? -1.0f : (down ? 1.0f : 0.0f);

    const bool keep_vert = (facing_ == Up && up) || (facing_ == Down && down);
    const bool keep_horiz = (facing_ == Left && left) || (facing_ == Right && right);
    if (!keep_vert && !keep_horiz) {
        if (down) {
            facing_ = Down;
        } else if (up) {
            facing_ = Up;
        } else if (right) {
            facing_ = Right;
        } else if (left) {
            facing_ = Left;
        }
    }

    const float move_speed = (debug_ && IsKeyDown(KEY_BACKSPACE)) ? (speed_ * 2.0f) : speed_;
    const float prev_x = x_;
    const float prev_y = y_;
    attempted_x_ = x_ + dx * move_speed;
    attempted_y_ = y_ + dy * move_speed;

    if (dx != 0.0f) {
        float next_x = x_ + dx * move_speed;
        if (!collides_at(next_x, y_)) {
            x_ = next_x;
        } else if (dy == 0.0f && !collides_at(next_x, y_ - move_speed)) {
            x_ = next_x;
            y_ -= move_speed;
        } else if (dy == 0.0f && !collides_at(next_x, y_ + move_speed)) {
            x_ = next_x;
            y_ += move_speed;
        }
    }

    if (dy != 0.0f) {
        float next_y = y_ + dy * move_speed;
        if (!collides_at(x_, next_y)) {
            y_ = next_y;
        } else if (dx == 0.0f && !collides_at(x_ - move_speed, next_y)) {
            x_ -= move_speed;
            y_ = next_y;
        } else if (dx == 0.0f && !collides_at(x_ + move_speed, next_y)) {
            x_ += move_speed;
            y_ = next_y;
        }
    }

    clamp_to_bounds();

    const bool moved = (x_ != prev_x) || (y_ != prev_y);
    const bool y_blocked = (up || down) && (y_ == prev_y);
    const bool murder_dance = up && down && y_blocked;

    if (murder_dance) {
        dance_toggle_ = !dance_toggle_;
        facing_ = dance_toggle_ ? Up : Down;
        frame_accum_ = moving_ ? (frame_accum_ + 0.2f) : 1.0f;
        moving_ = true;
    } else if (moved) {
        frame_accum_ = moving_ ? (frame_accum_ + 0.2f) : 1.0f;
        moving_ = true;
    } else {
        moving_ = false;
        frame_accum_ = 0.0f;
    }

    const int count = static_cast<int>(frames_[facing_].size());
    if (count > 0 && frame_accum_ >= static_cast<float>(count)) {
        frame_accum_ = std::fmod(frame_accum_, static_cast<float>(count));
    }
}

void OverworldFrisk::draw() {
    const auto& dir = frames_[facing_];
    if (dir.empty()) {
        return;
    }
    const std::size_t idx = static_cast<std::size_t>(frame_accum_) % dir.size();
    if (dir[idx].id > 0) {
        DrawTextureV(dir[idx], Vector2{x_, y_}, WHITE);
    }
}
