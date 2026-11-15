#pragma once

#include "entity/EntityGroup.hpp"

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class MenuSoul;
class TextEntity;

class SoulListMenu : public EntityGroup {
public:
    SoulListMenu(float x,
                 float y,
                 float spacing,
                 float soul_offset_x,
                 const std::string& font_path,
                 bool not_soul,
                 bool horizontal);
    ~SoulListMenu() override;

    void add_option(const std::string& label, luabridge::LuaRef on_select);
    void clear_options();
    void set_option_label(std::size_t index, const std::string& label);
    void set_option_color(std::size_t index, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void set_selected(std::size_t index);
    std::size_t selected() const;
    void on_move(luabridge::LuaRef callback);
    void set_font(const std::string& font_path);

protected:
    void self_step() override;

private:
    void update_colors();

    float x_ = 0.0f;
    float y_ = 0.0f;
    float spacing_ = 40.0f;
    float soul_offset_x_ = -28.0f;
    std::string font_path_;
    bool not_soul_ = false;
    bool horizontal_ = false;
    std::size_t selected_ = 0;
    std::shared_ptr<MenuSoul> soul_;
    std::vector<std::shared_ptr<TextEntity>> labels_;
    std::vector<luabridge::LuaRef> callbacks_;
    std::optional<luabridge::LuaRef> on_move_;
};
