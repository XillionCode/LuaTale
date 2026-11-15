#include "ImageEntity.hpp"

#include <algorithm>
#include <cstdint>

ImageEntity::ImageEntity(const std::string& path,
                         float x,
                         float y,
                         float scale,
                         float pan_dx,
                         float pan_dy,
                         float fade_speed)
    : x_(x),
      y_(y),
      scale_(scale > 0.0f ? scale : 1.0f),
      pan_dx_(pan_dx),
      pan_dy_(pan_dy),
      fade_speed_(fade_speed) {
    if (!path.empty()) {
        texture_ = LoadTexture(path.c_str());
        loaded_ = texture_.id != 0;
        if (loaded_) {
            SetTextureFilter(texture_, TEXTURE_FILTER_POINT);
        }
    }
    if (fade_speed_ <= 0.0f) {
        alpha_ = 1.0f;
        state_ = FadeState::Visible;
    }
}

ImageEntity::~ImageEntity() {
    if (loaded_) {
        UnloadTexture(texture_);
    }
}

void ImageEntity::step() {
    if (pan_delay_ > 0) {
        --pan_delay_;
    } else {
        x_ += pan_dx_;
        y_ += pan_dy_;
    }

    if (state_ == FadeState::FadeIn) {
        alpha_ = std::min(1.0f, alpha_ + fade_speed_);
        if (alpha_ >= 1.0f) {
            state_ = FadeState::Visible;
        }
    } else if (state_ == FadeState::FadeOut) {
        alpha_ = std::max(0.0f, alpha_ - fade_speed_);
        if (alpha_ <= 0.0f) {
            state_ = FadeState::Done;
        }
    }
}

void ImageEntity::draw() {
    if (!loaded_ || alpha_ <= 0.0f) {
        return;
    }

    const float draw_w = static_cast<float>(texture_.width) * scale_;
    const float draw_h = static_cast<float>(texture_.height) * scale_;

    Rectangle src{0.0f, 0.0f, static_cast<float>(texture_.width), static_cast<float>(texture_.height)};
    Rectangle dst{x_, y_, draw_w, draw_h};
    Color tint{255, 255, 255, static_cast<unsigned char>(alpha_ * 255.0f)};

    if (has_clip_) {
        BeginScissorMode(static_cast<int32_t>(clip_.x),
                         static_cast<int32_t>(clip_.y),
                         static_cast<int32_t>(clip_.width),
                         static_cast<int32_t>(clip_.height));
    }
    DrawTexturePro(texture_, src, dst, Vector2{0.0f, 0.0f}, 0.0f, tint);
    if (has_clip_) {
        EndScissorMode();
    }
}

void ImageEntity::set_clip(float x, float y, float w, float h) {
    has_clip_ = true;
    clip_ = Rectangle{x, y, w, h};
}

void ImageEntity::set_pan_delay(int32_t steps) {
    pan_delay_ = steps > 0 ? steps : 0;
}

void ImageEntity::start_fade_out() {
    if (state_ != FadeState::Done) {
        state_ = FadeState::FadeOut;
    }
}

bool ImageEntity::is_done() const {
    return state_ == FadeState::Done;
}
