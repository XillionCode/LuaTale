#pragma once

#include <cstddef>
#include <string>
#include <vector>

struct KeyItem {
    std::string label;
    float x = 0.0f;
    float y = 0.0f;
    bool shakes = true;
    bool is_action = false;
    int action_id = 0;  // 0=Quit, 1=Backspace, 2=Done
};

class KeyboardLayout {
public:
    void build(float x, float y);
    void handle_navigation();

    const std::vector<std::vector<KeyItem>>& rows() const;
    const KeyItem& current() const;
    std::size_t row() const;
    std::size_t col() const;

private:
    std::vector<std::vector<KeyItem>> rows_;
    std::size_t row_ = 0;
    std::size_t col_ = 0;
};
