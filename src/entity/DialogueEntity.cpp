#include "DialogueEntity.hpp"
#include "BoxEntity.hpp"
#include "SpriteEntity.hpp"
#include "TextEntity.hpp"

#include <iostream>
#include <raylib.h>
#include <utility>

DialogueEntity::DialogueEntity(bool top_side,
                               bool has_portrait,
                               const std::string& font_path,
                               const std::string& sfx_path,
                               float font_size,
                               int steps_per_char)
    : top_side_(top_side),
      has_portrait_(has_portrait) {
    box_ = std::make_shared<BoxEntity>(32.0f, 320.0f, 576.0f, 150.0f, 6.0f);
    portrait_ = std::make_shared<SpriteEntity>("", 56.0f, 346.0f, 2.0f);
    text_ = std::make_shared<TextEntity>("", 56.0f, 342.0f, font_path, sfx_path, font_size, steps_per_char);
    text_->set_spacing(16.0f, 36.0f);

    text_->set_cpp_emote_callback([this](int emote) {
        update_portrait_sprite(true);
        if (on_emote_ && on_emote_->isFunction()) {
            auto res = (*on_emote_)(emote);
            if (!res) {
                std::cerr << "DialogueEntity on_emote error: " << res.errorMessage() << '\n';
            }
        }
    });

    add_child(box_);
    add_child(portrait_);
    add_child(text_);
    update_layout();
}

DialogueEntity::~DialogueEntity() = default;

void DialogueEntity::update_layout() {
    const float box_y = top_side_ ? 10.0f : 320.0f;
    box_->set_pos(32.0f, box_y);
    portrait_->set_pos(56.0f, box_y + 26.0f);
    portrait_->set_visible(has_portrait_);
    text_->set_pos(has_portrait_ ? 172.0f : 56.0f, box_y + 22.0f);
}

void DialogueEntity::set_has_portrait(bool has_portrait) {
    has_portrait_ = has_portrait;
    update_layout();
}

void DialogueEntity::set_top_side(bool top_side) {
    top_side_ = top_side;
    update_layout();
}

void DialogueEntity::set_portrait(int emote_id, const std::string& closed_path, const std::string& open_path) {
    portraits_[emote_id] = PortraitImages{closed_path, open_path.empty() ? closed_path : open_path};
    if (!has_portrait_) {
        has_portrait_ = true;
        update_layout();
    }
    if (text_->current_emote() == emote_id || last_emote_ == -1) {
        update_portrait_sprite(true);
    }
}

void DialogueEntity::set_text(const std::string& raw) {
    pages_ = TextParser::parse(raw);
    page_index_ = 0;
    done_ = false;
    set_visible(true);
    set_active(true);
    show_current_page();
}

void DialogueEntity::add_page(const std::string& raw) {
    std::vector<ParsedPage> extra = TextParser::parse(raw);
    const bool was_empty = pages_.empty();
    for (ParsedPage& p : extra) {
        pages_.push_back(std::move(p));
    }
    if (was_empty && !pages_.empty()) {
        page_index_ = 0;
        done_ = false;
        show_current_page();
    }
}

void DialogueEntity::show_current_page() {
    if (page_index_ < pages_.size()) {
        text_->set_parsed_page(pages_[page_index_], false);
        mouth_timer_ = 0;
        last_mouth_open_ = false;
        update_portrait_sprite(true);
    }
}

void DialogueEntity::update_portrait_sprite(bool force) {
    if (!has_portrait_) {
        return;
    }
    const int emote = text_->current_emote();
    auto it = portraits_.find(emote);
    if (it == portraits_.end()) {
        it = portraits_.find(0);
        if (it == portraits_.end()) {
            return;
        }
    }

    const bool typing = text_->is_typing();
    bool mouth_open = false;
    if (typing) {
        ++mouth_timer_;
        mouth_open = ((mouth_timer_ / 4) % 2) == 1;
    } else {
        mouth_timer_ = 0;
    }

    if (!force && emote == last_emote_ && mouth_open == last_mouth_open_) {
        return;
    }

    last_emote_ = emote;
    last_mouth_open_ = mouth_open;
    const std::string& next_path = mouth_open ? it->second.open : it->second.closed;
    if (next_path != active_portrait_path_) {
        active_portrait_path_ = next_path;
        portrait_->set_texture(active_portrait_path_);
    }
}

void DialogueEntity::on_emote(luabridge::LuaRef callback) {
    if (callback.isFunction()) {
        on_emote_ = std::move(callback);
    } else {
        on_emote_.reset();
    }
}

void DialogueEntity::on_done(luabridge::LuaRef callback) {
    if (callback.isFunction()) {
        on_done_ = std::move(callback);
    } else {
        on_done_.reset();
    }
}

bool DialogueEntity::is_done() const {
    return done_;
}

void DialogueEntity::self_step() {
    if (done_) {
        return;
    }

    update_portrait_sprite(false);

    if (!text_->is_finished()) {
        if (IsKeyPressed(KEY_X) || IsKeyPressed(KEY_LEFT_SHIFT) || IsKeyPressed(KEY_RIGHT_SHIFT)) {
            text_->skip();
            update_portrait_sprite(true);
        }
        return;
    }

    if (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_ENTER)) {
        if (page_index_ + 1 < pages_.size()) {
            ++page_index_;
            show_current_page();
        } else {
            done_ = true;
            set_visible(false);
            set_active(false);
            if (on_done_ && on_done_->isFunction()) {
                auto res = (*on_done_)();
                if (!res) {
                    std::cerr << "DialogueEntity on_done error: " << res.errorMessage() << '\n';
                }
            }
        }
    }
}
