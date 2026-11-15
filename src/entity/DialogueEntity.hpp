#pragma once

#include "EntityGroup.hpp"
#include "text/Parser.hpp"

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

class BoxEntity;
class SpriteEntity;
class TextEntity;

class DialogueEntity : public EntityGroup {
public:
    DialogueEntity(bool top_side = false,
                   bool has_portrait = false,
                   const std::string& font_path = "fnt_maintext",
                   const std::string& sfx_path = "",
                   float font_size = 32.0f,
                   int32_t steps_per_char = 1);
    ~DialogueEntity() override;

    void set_text(const std::string& raw);
    void add_page(const std::string& raw);
    void set_has_portrait(bool has_portrait);
    void set_top_side(bool top_side);
    void set_portrait(int32_t emote_id, const std::string& closed_path, const std::string& open_path = "");
    void on_emote(luabridge::LuaRef callback);
    void on_done(luabridge::LuaRef callback);
    bool is_done() const;

protected:
    void self_step() override;

private:
    struct PortraitImages {
        std::string closed;
        std::string open;
    };

    void update_layout();
    void show_current_page();
    void update_portrait_sprite(bool force);

    std::shared_ptr<BoxEntity> box_;
    std::shared_ptr<SpriteEntity> portrait_;
    std::shared_ptr<TextEntity> text_;

    bool top_side_ = false;
    bool has_portrait_ = false;
    bool done_ = false;

    std::vector<ParsedPage> pages_;
    std::size_t page_index_ = 0;

    std::unordered_map<int32_t, PortraitImages> portraits_;
    int32_t last_emote_ = -1;
    bool last_mouth_open_ = false;
    int32_t mouth_timer_ = 0;
    std::string active_portrait_path_;

    std::optional<luabridge::LuaRef> on_emote_;
    std::optional<luabridge::LuaRef> on_done_;
};
