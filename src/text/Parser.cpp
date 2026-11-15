#include "Parser.hpp"

#include <cctype>

namespace {

bool color_from_code(char code, Color& out) {
    switch (code) {
        case 'W':
            out = WHITE;
            return true;
        case 'R':
            out = RED;
            return true;
        case 'Y':
            out = YELLOW;
            return true;
        case 'G':
            out = GREEN;
            return true;
        case 'B':
            out = Color{64, 160, 255, 255};
            return true;
        case 'O':
            out = ORANGE;
            return true;
        case 'P':
            out = Color{213, 128, 255, 255};
            return true;
        case 'X':
            out = BLACK;
            return true;
        default:
            return false;
    }
}

void finalize_color_runs(ParsedPage& page) {
    page.color_runs.clear();
    Color current = WHITE;
    std::size_t run_start = 0;

    for (const TextEvent& ev : page.events) {
        if (ev.type != TextEvent::Type::SetColor) {
            continue;
        }
        if (ev.char_index > run_start) {
            page.color_runs.push_back(TextColorRun{run_start, ev.char_index - run_start, current});
        }
        current = ev.color;
        run_start = ev.char_index;
    }
    if (page.text.size() > run_start || page.color_runs.empty()) {
        page.color_runs.push_back(TextColorRun{run_start, page.text.size() - run_start, current});
    }
}

}  // namespace

std::vector<ParsedPage> TextParser::parse(const std::string& raw) {
    std::vector<ParsedPage> pages;
    ParsedPage current;

    auto flush_page = [&]() {
        if (!current.text.empty() || !current.events.empty()) {
            finalize_color_runs(current);
            pages.push_back(std::move(current));
            current = ParsedPage{};
        }
    };

    for (std::size_t i = 0; i < raw.size(); ++i) {
        const char c = raw[i];

        if (c == '&' || c == '#') {
            current.text.push_back('\n');
            continue;
        }

        if (c == '^' && i + 1 < raw.size() && raw[i + 1] >= '1' && raw[i + 1] <= '9') {
            TextEvent ev;
            ev.type = TextEvent::Type::Pause;
            ev.char_index = current.text.size();
            ev.value = (raw[i + 1] - '0') * 4;
            current.events.push_back(ev);
            ++i;
            continue;
        }

        if (c == '\\' && i + 1 < raw.size()) {
            const char code = raw[i + 1];
            if (code == 'E' && i + 2 < raw.size() && std::isdigit(static_cast<unsigned char>(raw[i + 2]))) {
                TextEvent ev;
                ev.type = TextEvent::Type::Emote;
                ev.char_index = current.text.size();
                ev.value = raw[i + 2] - '0';
                current.events.push_back(ev);
                i += 2;
                continue;
            }
            Color col{};
            if (color_from_code(code, col)) {
                TextEvent ev;
                ev.type = TextEvent::Type::SetColor;
                ev.char_index = current.text.size();
                ev.color = col;
                current.events.push_back(ev);
                ++i;
                continue;
            }
            // Strip other Undertale control flags like \M0, \F1, \C, \T, etc.
            if ((code == 'M' || code == 'F' || code == 'S' || code == 'T') &&
                i + 2 < raw.size() && std::isdigit(static_cast<unsigned char>(raw[i + 2]))) {
                i += 2;
            } else {
                ++i;
            }
            continue;
        }

        if (c == '/') {
            if (i + 1 < raw.size() && raw[i + 1] == '%') {
                ++i;
            }
            flush_page();
            continue;
        }

        if (c == '%') {
            if (i + 1 < raw.size() && raw[i + 1] == '%') {
                ++i;
            }
            flush_page();
            continue;
        }

        current.text.push_back(c);
    }

    flush_page();
    if (pages.empty()) {
        finalize_color_runs(current);
        pages.push_back(std::move(current));
    }
    return pages;
}

ParsedPage TextParser::parse_single(const std::string& raw) {
    std::vector<ParsedPage> pages = parse(raw);
    if (pages.size() == 1) {
        return std::move(pages[0]);
    }
    ParsedPage combined;
    for (std::size_t p = 0; p < pages.size(); ++p) {
        if (p > 0 && !pages[p].text.empty()) {
            combined.text.push_back('\n');
        }
        const std::size_t offset = combined.text.size();
        combined.text += pages[p].text;
        for (TextEvent ev : pages[p].events) {
            ev.char_index += offset;
            combined.events.push_back(ev);
        }
    }
    finalize_color_runs(combined);
    return combined;
}
