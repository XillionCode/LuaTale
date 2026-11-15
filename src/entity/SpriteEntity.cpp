#include "SpriteEntity.hpp"

SpriteEntity::SpriteEntity(const std::string& sprite_path, float x, float y, float scale)
    : x_(x),
      y_(y),
      scale_(scale > 0.0f ? scale : 1.0f) {
    set_texture(sprite_path);
}

SpriteEntity::~SpriteEntity() {
    if (loaded_) {
        UnloadTexture(texture_);
    }
}

void SpriteEntity::set_texture(const std::string& sprite_path) {
    if (loaded_) {
        UnloadTexture(texture_);
        texture_ = Texture2D{};
        loaded_ = false;
    }
    if (!sprite_path.empty()) {
        texture_ = LoadTexture(sprite_path.c_str());
        loaded_ = texture_.id != 0;
        if (loaded_) {
            SetTextureFilter(texture_, TEXTURE_FILTER_POINT);
        }
    }
}

void SpriteEntity::draw() {
    if (!loaded_) {
        return;
    }

    Rectangle src{0.0f, 0.0f, static_cast<float>(texture_.width), static_cast<float>(texture_.height)};
    Rectangle dst{x_, y_, static_cast<float>(texture_.width) * scale_, static_cast<float>(texture_.height) * scale_};
    DrawTexturePro(texture_, src, dst, Vector2{0.0f, 0.0f}, 0.0f, tint_);
}

void SpriteEntity::set_pos(float x, float y) {
    x_ = x;
    y_ = y;
}

float SpriteEntity::x() const {
    return x_;
}

float SpriteEntity::y() const {
    return y_;
}

float SpriteEntity::width() const {
    return loaded_ ? static_cast<float>(texture_.width) * scale_ : 0.0f;
}

float SpriteEntity::height() const {
    return loaded_ ? static_cast<float>(texture_.height) * scale_ : 0.0f;
}

void SpriteEntity::set_tint(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    tint_ = Color{r, g, b, a};
}
