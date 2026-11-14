#pragma once

#include <string>
#include <unordered_map>

class TextData {
public:
    void load(const std::string& lang, const std::string& path);
    void load_naming_responses(const std::string& path);
    std::string get(const std::string& lang, const std::string& key) const;

private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> tables_;
};
