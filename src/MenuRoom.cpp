#include "MenuRoom.hpp"
#include "OptionMenu.hpp"

#include <utility>

MenuRoom::MenuRoom(float center_x,
                   float top_y,
                   float spread_x,
                   float spread_y,
                   const std::string& font_path) {
    menu_ = std::make_shared<OptionMenu>(center_x, top_y, spread_x, spread_y, font_path);
    add_entity(menu_);
}

MenuRoom::~MenuRoom() = default;

void MenuRoom::add_option(const std::string& label, luabridge::LuaRef on_select) {
    menu_->add_option(label, std::move(on_select));
}

std::shared_ptr<OptionMenu> MenuRoom::menu() const {
    return menu_;
}
