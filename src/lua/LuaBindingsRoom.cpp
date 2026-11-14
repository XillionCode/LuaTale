#include "LuaBindings.hpp"
#include "entity/Entity.hpp"
#include "entity/ImageEntity.hpp"
#include "room/MenuRoom.hpp"
#include "menu/OptionMenu.hpp"
#include "room/Room.hpp"
#include "room/RoomCutscene.hpp"
#include "entity/TextEntity.hpp"

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <memory>
#include <string>

void register_room_bindings(lua_State* lua) {
    luabridge::getGlobalNamespace(lua)
        .beginClass<Room>("Room")
        .addConstructorFrom<std::shared_ptr<Room>, void()>()
        .addFunction("add_entity", &Room::add_entity)
        .endClass()
        .deriveClass<RoomCutscene, Room>("RoomCutscene")
        .addConstructorFrom<std::shared_ptr<RoomCutscene>,
                            void(const std::string&, float, float, float, float, float, float,
                                 const std::string&, float, float, const std::string&, const std::string&,
                                 int, luabridge::LuaRef, luabridge::LuaRef)>()
        .addFunction("image", &RoomCutscene::image)
        .addFunction("text", &RoomCutscene::text)
        .addFunction("set_debug", &RoomCutscene::set_debug)
        .endClass()
        .deriveClass<MenuRoom, Room>("MenuRoom")
        .addConstructorFrom<std::shared_ptr<MenuRoom>, void(float, float, float, float, const std::string&)>()
        .addFunction("add_option", &MenuRoom::add_option)
        .addFunction("menu", &MenuRoom::menu)
        .endClass();
}
