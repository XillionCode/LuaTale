#pragma once

#include <cstddef>
#include <cstdint>
#include <raylib.h>
#include <string>
#include <vector>

struct TextColorRun {
    std::size_t start = 0;
    std::size_t length = 0;
    Color color = WHITE;
};

struct TextEvent {
    enum class Type {
        Pause,
        Emote,
        SetColor,
    };

    Type type = Type::Pause;
    std::size_t char_index = 0;
    int32_t value = 0;
    Color color = WHITE;
};

struct ParsedPage {
    std::string text;
    std::vector<TextColorRun> color_runs;
    std::vector<TextEvent> events;
};

class TextParser {
public:
    static std::vector<ParsedPage> parse(const std::string& raw);
    static ParsedPage parse_single(const std::string& raw);
};
