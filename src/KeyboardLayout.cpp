#include "KeyboardLayout.hpp"

#include <raylib.h>
#include <algorithm>
#include <utility>

void KeyboardLayout::build(float x, float y) {
    rows_.clear();

    const std::vector<std::string> alpha_rows = {
        "ABCDEFG",
        "HIJKLMN",
        "OPQRSTU",
        "VWXYZ",
        "abcdefg",
        "hijklmno",
        "pqrstuv",
        "wxyz",
    };

    constexpr float kColStep = 64.0f;
    constexpr float kRowStep = 28.0f;

    for (std::size_t r = 0; r < alpha_rows.size(); ++r) {
        std::vector<KeyItem> row;
        const float y_off = (r >= 4 ? 12.0f : 0.0f);
        for (std::size_t c = 0; c < alpha_rows[r].size(); ++c) {
            row.push_back(KeyItem{
                std::string(1, alpha_rows[r][c]),
                x + static_cast<float>(c) * kColStep,
                y + static_cast<float>(r) * kRowStep + y_off,
                true,
                false,
                0,
            });
        }
        rows_.push_back(std::move(row));
    }

    const float action_y = y + static_cast<float>(alpha_rows.size()) * kRowStep + 24.0f;
    rows_.push_back({
        KeyItem{"Quit", x, action_y, false, true, 0},
        KeyItem{"Backspace", x + 130.0f, action_y, false, true, 1},
        KeyItem{"Done", x + 320.0f, action_y, false, true, 2},
    });
}

void KeyboardLayout::handle_navigation() {
    if (rows_.empty()) {
        return;
    }

    if (IsKeyPressed(KEY_UP)) {
        row_ = (row_ == 0) ? (rows_.size() - 1) : (row_ - 1);
        col_ = std::min(col_, rows_[row_].size() - 1);
    } else if (IsKeyPressed(KEY_DOWN)) {
        row_ = (row_ + 1) % rows_.size();
        col_ = std::min(col_, rows_[row_].size() - 1);
    } else if (IsKeyPressed(KEY_LEFT)) {
        const std::size_t cols = rows_[row_].size();
        col_ = (col_ == 0) ? (cols - 1) : (col_ - 1);
    } else if (IsKeyPressed(KEY_RIGHT)) {
        col_ = (col_ + 1) % rows_[row_].size();
    }
}

const std::vector<std::vector<KeyItem>>& KeyboardLayout::rows() const {
    return rows_;
}

const KeyItem& KeyboardLayout::current() const {
    return rows_[row_][col_];
}

std::size_t KeyboardLayout::row() const {
    return row_;
}

std::size_t KeyboardLayout::col() const {
    return col_;
}
