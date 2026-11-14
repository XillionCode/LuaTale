#pragma once

#include "SoulListMenu.hpp"

#include <string>

class SoulVertMenu : public SoulListMenu {
public:
    SoulVertMenu(float x,
                 float y,
                 float spacing = 40.0f,
                 float soul_offset_x = -28.0f,
                 const std::string& font_path = "fnt_maintext",
                 bool not_soul = false);
    ~SoulVertMenu() override;
};
