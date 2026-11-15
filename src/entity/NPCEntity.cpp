#include "NPCEntity.hpp"

#include <cmath>
#include <cstdint>
#include <iostream>
#include <utility>

NPCEntity::NPCEntity(const std::string& sprite_path, float x, float y, float scale)
    : SpriteEntity(sprite_path, x, y, scale) {
    if (!sprite_path.empty() && loaded_) {
        frames_.push_back(texture_);
        loaded_ = false;
        texture_ = Texture2D{};
    }
}

NPCEntity::~NPCEntity() {
    for (Texture2D& tex : frames_) {
        if (tex.id != 0) {
            UnloadTexture(tex);
        }
    }
}

void NPCEntity::add_frame(const std::string& path) {
    if (path.empty()) {
        return;
    }
    Texture2D tex = LoadTexture(path.c_str());
    if (tex.id != 0) {
        SetTextureFilter(tex, TEXTURE_FILTER_POINT);
        frames_.push_back(tex);
    }
}

void NPCEntity::set_frame(int32_t index) {
    if (frames_.empty()) {
        return;
    }
    const int32_t count = static_cast<int32_t>(frames_.size());
    current_frame_ = ((index % count) + count) % count;
    frame_accum_ = static_cast<float>(current_frame_);
}

void NPCEntity::set_anim_speed(float frames_per_step) {
    anim_speed_ = frames_per_step;
}

void NPCEntity::set_solid(bool solid) {
    solid_ = solid;
}

bool NPCEntity::solid() const {
    return solid_;
}

Rectangle NPCEntity::hitbox() const {
    if (!frames_.empty()) {
        const Texture2D& tex = frames_[static_cast<std::size_t>(current_frame_)];
        return Rectangle{x_, y_, static_cast<float>(tex.width) * scale_, static_cast<float>(tex.height) * scale_};
    }
    return Rectangle{x_, y_, width(), height()};
}

void NPCEntity::on_interact(luabridge::LuaRef callback) {
    if (callback.isFunction()) {
        on_interact_ = std::move(callback);
    } else {
        on_interact_.reset();
    }
}

void NPCEntity::trigger_interact() {
    if (on_interact_ && on_interact_->isFunction()) {
        auto res = (*on_interact_)();
        if (!res) {
            std::cerr << "NPCEntity on_interact error: " << res.errorMessage() << '\n';
        }
    }
}

void NPCEntity::walk_to(float target_x, float target_y, float speed, luabridge::LuaRef on_arrive) {
    walking_ = true;
    walk_target_x_ = target_x;
    walk_target_y_ = target_y;
    walk_speed_ = speed > 0.0f ? speed : 2.0f;
    if (on_arrive.isFunction()) {
        on_arrive_ = std::move(on_arrive);
    } else {
        on_arrive_.reset();
    }
}

void NPCEntity::step() {
    if (anim_speed_ > 0.0f && !frames_.empty()) {
        frame_accum_ += anim_speed_;
        const int32_t count = static_cast<int32_t>(frames_.size());
        if (frame_accum_ >= static_cast<float>(count)) {
            frame_accum_ = std::fmod(frame_accum_, static_cast<float>(count));
        }
        current_frame_ = static_cast<int32_t>(frame_accum_);
    }

    if (walking_) {
        const float dx = walk_target_x_ - x_;
        const float dy = walk_target_y_ - y_;
        const float dist = std::hypot(dx, dy);
        if (dist <= walk_speed_) {
            x_ = walk_target_x_;
            y_ = walk_target_y_;
            walking_ = false;
            if (on_arrive_ && on_arrive_->isFunction()) {
                auto cb = std::move(*on_arrive_);
                on_arrive_.reset();
                auto res = cb();
                if (!res) {
                    std::cerr << "NPCEntity on_arrive error: " << res.errorMessage() << '\n';
                }
            }
        } else {
            x_ += (dx / dist) * walk_speed_;
            y_ += (dy / dist) * walk_speed_;
        }
    }
}

void NPCEntity::draw() {
    if (frames_.empty()) {
        SpriteEntity::draw();
        return;
    }
    const Texture2D& tex = frames_[static_cast<std::size_t>(current_frame_)];
    if (tex.id == 0) {
        return;
    }
    Rectangle src{0.0f, 0.0f, static_cast<float>(tex.width), static_cast<float>(tex.height)};
    Rectangle dst{x_, y_, static_cast<float>(tex.width) * scale_, static_cast<float>(tex.height) * scale_};
    DrawTexturePro(tex, src, dst, Vector2{0.0f, 0.0f}, 0.0f, tint_);
}
