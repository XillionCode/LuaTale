#pragma once

#include "SpriteEntity.hpp"

#include <string>

class Soul : public SpriteEntity {
public:
    Soul(float x = 0.0f,
         float y = 0.0f,
         const std::string& sprite_path = "assets/Sprites/spr_heart_0.png",
         float scale = 2.0f);
    ~Soul() override;

protected:
    bool up_pressed() const;
    bool down_pressed() const;
    bool left_pressed() const;
    bool right_pressed() const;

    bool up_held() const;
    bool down_held() const;
    bool left_held() const;
    bool right_held() const;
};
