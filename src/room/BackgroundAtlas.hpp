#pragma once

#include <cstdint>
#include <raylib.h>
#include <string>
#include <unordered_map>

struct BackgroundEntry {
    int32_t texture_page = -1;
    float source_x = 0.0f;
    float source_y = 0.0f;
    float source_width = 0.0f;
    float source_height = 0.0f;
};

class BackgroundAtlas {
public:
    BackgroundAtlas() = default;
    ~BackgroundAtlas();

    BackgroundAtlas(const BackgroundAtlas&) = delete;
    BackgroundAtlas& operator=(const BackgroundAtlas&) = delete;

    bool load(const std::string& json_path);
    const BackgroundEntry* find(const std::string& name) const;
    Texture2D texture_for_page(int32_t page);

private:
    std::unordered_map<std::string, BackgroundEntry> entries_;
    std::unordered_map<int32_t, Texture2D> pages_;
};
