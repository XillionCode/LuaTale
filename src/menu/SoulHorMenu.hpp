#pragma once

#include "SoulListMenu.hpp"

#include <string>

class SoulHorMenu : public SoulListMenu {
public:
    SoulHorMenu(float x,
                float y,
                float spacing = 160.0f,
                float soul_offset_x = -28.0f,
                const std::string& font_path = "fnt_maintext",
                bool not_soul = false);
    ~SoulHorMenu() override;
};
