#include "TextData.hpp"

#include <fstream>
#include <unordered_set>
#include <vector>

namespace {

std::string trim_cr(std::string s) {
    if (!s.empty() && s.back() == '\r') {
        s.pop_back();
    }
    return s;
}

bool is_ascii(const std::string& s) {
    for (unsigned char c : s) {
        if (c >= 0x80) {
            return false;
        }
    }
    return true;
}

}  // namespace

void TextData::load(const std::string& lang, const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        return;
    }

    auto& map = tables_[lang];
    std::string line;
    while (std::getline(file, line)) {
        const std::size_t first_quote = line.find('"');
        if (first_quote == std::string::npos) {
            continue;
        }
        const std::size_t second_quote = line.find('"', first_quote + 1);
        if (second_quote == std::string::npos) {
            continue;
        }
        const std::size_t third_quote = line.find('"', second_quote + 1);
        if (third_quote == std::string::npos) {
            continue;
        }
        const std::size_t end_paren = line.rfind("\");");
        if (end_paren == std::string::npos || end_paren <= third_quote) {
            continue;
        }

        std::string key = line.substr(first_quote + 1, second_quote - first_quote - 1);
        std::string val = line.substr(third_quote + 1, end_paren - third_quote - 1);
        map[key] = val;
    }
}

std::string TextData::get(const std::string& lang, const std::string& key) const {
    auto lang_it = tables_.find(lang);
    if (lang_it != tables_.end()) {
        auto it = lang_it->second.find(key);
        if (it != lang_it->second.end()) {
            return it->second;
        }
    }
    auto en_it = tables_.find("en");
    if (en_it != tables_.end()) {
        auto it = en_it->second.find(key);
        if (it != en_it->second.end()) {
            return it->second;
        }
    }
    return key;
}

void TextData::load_naming_responses(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        return;
    }

    std::string line;
    bool in_block = false;
    while (std::getline(file, line)) {
        if (trim_cr(line) == "l_char") {
            std::string next;
            if (std::getline(file, next) && trim_cr(next) == "aaaaaa") {
                in_block = true;
                break;
            }
        }
    }
    if (!in_block) {
        return;
    }

    std::vector<std::string> block = {"aaaaaa"};
    while (std::getline(file, line)) {
        line = trim_cr(line);
        if (line == "gaster" || line == "name_entry_confirm") {
            break;
        }
        block.push_back(line);
    }

    static const std::unordered_set<std::string> triggers = {
        "aaaaaa", "asgore", "toriel", "sans",   "undyne", "flowey",
        "chara",  "alphys", "alphy",  "papyru", "napsta", "blooky",
        "ころす", "asriel", "catty",  "bratty", "mtt",    "metta",
        "mett",   "gerson", "shyren", "aaron",  "temmie", "woshua",
        "jerry",  "bpants",
    };

    auto commit_group = [&](const std::vector<std::string>& keys,
                            const std::vector<std::string>& payload) {
        if (keys.empty()) {
            return;
        }
        std::string msg_en;
        std::string msg_ja;
        for (const auto& s : payload) {
            if (is_ascii(s)) {
                msg_en = s;
            } else {
                msg_ja = s;
            }
        }
        if (msg_en.empty() && keys[0] == "asriel") {
            msg_en = "...";
        }
        for (const auto& k : keys) {
            if (k == "ころす") {
                tables_["en"]["name_resp_murder"] = msg_en;
                tables_["en"]["name_resp_mercy"]  = msg_en;
                tables_["ja"]["name_resp_murder"] = msg_ja;
                tables_["ja"]["name_resp_mercy"]  = msg_ja;
            } else {
                tables_["en"]["name_resp_" + k] = msg_en;
                tables_["ja"]["name_resp_" + k] = msg_ja;
            }
        }
    };

    std::vector<std::string> keys;
    std::vector<std::string> payload;
    for (const auto& s : block) {
        if (triggers.count(s)) {
            if (!payload.empty()) {
                commit_group(keys, payload);
                keys.clear();
                payload.clear();
            }
            keys.push_back(s);
        } else {
            payload.push_back(s);
        }
    }
    commit_group(keys, payload);
}
