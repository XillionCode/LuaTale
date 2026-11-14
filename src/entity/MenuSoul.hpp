#pragma once

#include "Soul.hpp"

#include <cstddef>
#include <vector>

class MenuSoul : public Soul {
public:
    MenuSoul(float x = 0.0f, float y = 0.0f, bool vert_lock = false, bool hor_lock = true);
    ~MenuSoul() override;

    void step() override;

    void add_snap_point(float pos);
    std::size_t get_selected() const;

private:
    void apply_snap();

    bool vert_lock_ = false;
    bool hor_lock_ = true;
    std::vector<float> snap_points_;
    std::size_t selected_ = 0;
};
