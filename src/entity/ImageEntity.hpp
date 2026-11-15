#pragma once

#include "Entity.hpp"

#include <cstdint>
#include <raylib.h>
#include <string>

class ImageEntity : public Entity {
public:
    enum class FadeState {
        FadeIn,
        Visible,
        FadeOut,
        Done
    };

    ImageEntity(const std::string& path,
                float x,
                float y,
                float scale = 2.0f,
                float pan_dx = 0.0f,
                float pan_dy = 0.0f,
                float fade_speed = 0.05f);
    ~ImageEntity() override;

    void step() override;
    void draw() override;

    void start_fade_out();
    bool is_done() const;
    void set_clip(float x, float y, float w, float h);
    void set_pan_delay(int32_t steps);

private:
    Texture2D texture_{};
    bool loaded_ = false;
    float x_ = 0.0f;
    float y_ = 0.0f;
    float scale_ = 2.0f;
    float pan_dx_ = 0.0f;
    float pan_dy_ = 0.0f;
    float alpha_ = 0.0f;
    float fade_speed_ = 0.05f;
    FadeState state_ = FadeState::FadeIn;
    bool has_clip_ = false;
    Rectangle clip_{};
    int32_t pan_delay_ = 0;
};
