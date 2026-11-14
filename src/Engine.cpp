#include "Engine.hpp"
#include "LuaBindings.hpp"
#include "Room.hpp"

#include <raylib.h>
#include <lua.hpp>
#include <iostream>
#include <utility>

Engine::Engine() {
    constexpr int kWidth = 640;
    constexpr int kHeight = 480;
    constexpr int kTargetFps = 30;

    InitWindow(kWidth, kHeight, "UNDERTALE");
    InitAudioDevice();
    SetTargetFPS(kTargetFps);

    lua_ = luaL_newstate();
    luaL_openlibs(lua_);

    load_strings();
    register_lua();

    if (luaL_dofile(lua_, "scripts/main.lua") != LUA_OK) {
        std::cerr << "Lua error: " << lua_tostring(lua_, -1) << '\n';
        lua_pop(lua_, 1);
    }
}

Engine::~Engine() {
    current_room_.reset();
    music_.stop();
    if (lua_ != nullptr) {
        lua_close(lua_);
        lua_ = nullptr;
    }
    CloseAudioDevice();
    CloseWindow();
}

void Engine::set_room(std::shared_ptr<Room> room) {
    current_room_ = std::move(room);
}

void Engine::set_title(const std::string& title) {
    SetWindowTitle(title.c_str());
}

void Engine::set_language(const std::string& lang) {
    language_ = lang;
}

const std::string& Engine::language() const {
    return language_;
}

std::string Engine::get_text(const std::string& key) const {
    return text_data_.get(language_, key);
}

void Engine::load_strings() {
    text_data_.load("en", "gml/CodeEntries/gml_Script_textdata_en.gml");
    text_data_.load("ja", "gml/CodeEntries/gml_Script_textdata_ja.gml");
    text_data_.load_naming_responses("assets/strings.txt");
}

void Engine::quit() {
    should_quit_ = true;
}

void Engine::register_lua() {
    register_lua_bindings(lua_, this, &music_);
}

void Engine::run() {
    while (!WindowShouldClose() && !should_quit_) {
        music_.update();

        if (current_room_) {
            current_room_->step();
        }

        BeginDrawing();
        if (current_room_) {
            current_room_->draw();
        } else {
            ClearBackground(BLACK);
        }
        EndDrawing();
    }
}
