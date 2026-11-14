#pragma once

class Engine;
class MusicPlayer;
struct lua_State;

void register_entity_bindings(lua_State* lua);
void register_room_bindings(lua_State* lua);
void register_lua_bindings(lua_State* lua, Engine* engine, MusicPlayer* music);
