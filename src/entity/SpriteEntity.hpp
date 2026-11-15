#pragma once

#include "Entity.hpp"

#include <raylib.h>
#include <string>

class SpriteEntity : public Entity {
public:
    SpriteEntity(const std::string& sprite_path, float x = 0.0f, float y = 0.0f, float scale = 2.0f);
    ~SpriteEntity() override;

    void draw() override;

    void set_pos(float x, float y);
    void set_texture(const std::string& sprite_path);
    float x() const;
    float y() const;
    float width() const;
    float height() const;
    void set_tint(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

protected:
    float x_ = 0.0f;
    float y_ = 0.0f;
    float scale_ = 2.0f;
    Texture2D texture_{};
    bool loaded_ = false;
    Color tint_ = WHITE;
};
