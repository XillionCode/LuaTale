#pragma once

#include "MusicPlayer.hpp"
#include "TextData.hpp"

#include <memory>
#include <string>

class Room;
struct lua_State;

class Engine {
public:
    Engine();
    ~Engine();

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    void set_room(std::shared_ptr<Room> room);
    void set_title(const std::string& title);
    void set_language(const std::string& lang);
    void set_log_level(const std::string& level);
    const std::string& language() const;
    std::string get_text(const std::string& key) const;
    void quit();
    void run();

private:
    void register_lua();
    void load_strings();

    lua_State* lua_ = nullptr;
    MusicPlayer music_;
    std::shared_ptr<Room> current_room_;
    std::string language_ = "en";
    TextData text_data_;
    bool should_quit_ = false;
};
