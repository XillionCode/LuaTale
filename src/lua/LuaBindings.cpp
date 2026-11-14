#include "LuaBindings.hpp"
#include "core/Engine.hpp"
#include "core/MusicPlayer.hpp"
#include "room/Room.hpp"

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

void register_lua_bindings(lua_State* lua, Engine* engine, MusicPlayer* music) {
    register_entity_bindings(lua);
    register_room_bindings(lua);

    luabridge::getGlobalNamespace(lua)
        .beginClass<MusicPlayer>("MusicPlayer")
        .addFunction("start_song", &MusicPlayer::start_song)
        .addFunction("play_sfx", &MusicPlayer::play_sfx)
        .addFunction("stop", &MusicPlayer::stop)
        .addFunction("set_pitch", &MusicPlayer::set_pitch)
        .addFunction("set_volume", &MusicPlayer::set_volume)
        .endClass()
        .beginClass<Engine>("Engine")
        .addFunction("set_room", &Engine::set_room)
        .addFunction("set_title", &Engine::set_title)
        .addFunction("set_language", &Engine::set_language)
        .addFunction("set_log_level", &Engine::set_log_level)
        .addFunction("language", &Engine::language)
        .addFunction("get_text", &Engine::get_text)
        .addFunction("quit", &Engine::quit)
        .endClass();

    luabridge::setGlobal(lua, engine, "engine");
    luabridge::setGlobal(lua, music, "music");
}
