#pragma once

#include "EntityGroup.hpp"

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

class TextEntity;

class OptionMenu : public EntityGroup {
public:
    OptionMenu(float center_x = 320.0f,
               float top_y = 260.0f,
               float spread_x = 120.0f,
               float spread_y = 44.0f,
               const std::string& font_path = "fnt_maintext");
    ~OptionMenu() override;

    void add_option(const std::string& label, luabridge::LuaRef on_select);

protected:
    void self_step() override;

private:
    void relayout();
    void update_colors();

    float center_x_ = 320.0f;
    float top_y_ = 260.0f;
    float spread_x_ = 120.0f;
    float spread_y_ = 44.0f;
    std::string font_path_;
    std::vector<std::shared_ptr<TextEntity>> labels_;
    std::vector<luabridge::LuaRef> callbacks_;
    std::size_t selected_ = 0;
};
