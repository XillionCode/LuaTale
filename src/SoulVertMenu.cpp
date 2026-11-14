#include "SoulVertMenu.hpp"

SoulVertMenu::SoulVertMenu(float x,
                           float y,
                           float spacing,
                           float soul_offset_x,
                           const std::string& font_path,
                           bool not_soul)
    : SoulListMenu(x, y, spacing, soul_offset_x, font_path, not_soul, false) {}

SoulVertMenu::~SoulVertMenu() = default;
