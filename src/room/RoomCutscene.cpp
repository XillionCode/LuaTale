#include "RoomCutscene.hpp"
#include "entity/ImageEntity.hpp"
#include "entity/TextEntity.hpp"

#include <raylib.h>
#include <iostream>
#include <utility>

RoomCutscene::RoomCutscene(const std::string& image_path,
                           float img_x,
                           float img_y,
                           float img_scale,
                           float pan_dx,
                           float pan_dy,
                           float fade_speed,
                           const std::string& text,
                           float text_x,
                           float text_y,
                           const std::string& font_path,
                           const std::string& sfx_path,
                           int hold_steps,
                           luabridge::LuaRef on_end,
                           luabridge::LuaRef on_skip)
    : hold_steps_(hold_steps),
      on_end_(std::move(on_end)),
      on_skip_(std::move(on_skip)) {
    image_ = std::make_shared<ImageEntity>(image_path, img_x, img_y, img_scale, pan_dx, pan_dy, fade_speed);
    add_entity(image_);

    if (!text.empty()) {
        text_ = std::make_shared<TextEntity>(text, text_x, text_y, font_path, sfx_path, 32.0f, 2);
        add_entity(text_);
    }
}

RoomCutscene::~RoomCutscene() = default;

std::shared_ptr<ImageEntity> RoomCutscene::image() const {
    return image_;
}

std::shared_ptr<TextEntity> RoomCutscene::text() const {
    return text_;
}

void RoomCutscene::set_debug(bool debug) {
    debug_ = debug;
}

void RoomCutscene::step() {
    Room::step();

    if (ended_) {
        return;
    }

    if (debug_ && IsKeyPressed(KEY_N)) {
        ended_ = true;
        if (on_end_.isFunction()) {
            auto res = on_end_();
            if (!res) {
                std::cerr << "RoomCutscene on_end error: " << res.errorMessage() << '\n';
            }
        }
        return;
    }

    const bool advance_pressed = IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_ENTER);

    if (!ending_) {
        if (advance_pressed) {
            ending_ = true;
            skipped_ = true;
            image_->start_fade_out();
        } else if (hold_steps_ >= 0 && (!text_ || text_->is_finished())) {
            ++hold_timer_;
            if (hold_timer_ >= hold_steps_) {
                ending_ = true;
                image_->start_fade_out();
            }
        }
    } else if (image_->is_done()) {
        ended_ = true;
        luabridge::LuaRef& cb = (skipped_ && on_skip_.isFunction()) ? on_skip_ : on_end_;
        if (cb.isFunction()) {
            auto res = cb();
            if (!res) {
                std::cerr << "RoomCutscene callback error: " << res.errorMessage() << '\n';
            }
        }
    }
}
