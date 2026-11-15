#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

struct KeyItem {
    std::string label;
    float x = 0.0f;
    float y = 0.0f;
    bool shakes = true;
    bool is_action = false;
    int32_t action_id = 0;  // 0=Quit, 1=Backspace, 2=Done
};

class KeyboardLayout {
public:
    static constexpr std::size_t kRowCount = 9;

    void build(float x, float y);
    void handle_navigation();

    const std::array<std::vector<KeyItem>, kRowCount>& rows() const;
    const KeyItem& current() const;
    std::size_t row() const;
    std::size_t col() const;

private:
    std::array<std::vector<KeyItem>, kRowCount> rows_;
    std::size_t row_ = 0;
    std::size_t col_ = 0;
};
