#include "LuaBindings.hpp"
#include "entity/Entity.hpp"
#include "entity/EntityGroup.hpp"
#include "entity/ImageEntity.hpp"
#include "menu/KeyboardEntity.hpp"
#include "entity/MenuSoul.hpp"
#include "menu/OptionMenu.hpp"
#include "entity/Soul.hpp"
#include "menu/SoulHorMenu.hpp"
#include "menu/SoulListMenu.hpp"
#include "menu/SoulVertMenu.hpp"
#include "entity/SpriteEntity.hpp"
#include "entity/TextEntity.hpp"

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <memory>
#include <string>

void register_entity_bindings(lua_State* lua) {
    luabridge::getGlobalNamespace(lua)
        .beginClass<Entity>("Entity")
        .addConstructorFrom<std::shared_ptr<Entity>, void()>()
        .endClass()
        .deriveClass<EntityGroup, Entity>("EntityGroup")
        .addConstructorFrom<std::shared_ptr<EntityGroup>, void()>()
        .addFunction("add_child", &EntityGroup::add_child)
        .endClass()
        .deriveClass<SoulListMenu, EntityGroup>("SoulListMenu")
        .addFunction("add_option", &SoulListMenu::add_option)
        .addFunction("set_option_label", &SoulListMenu::set_option_label)
        .addFunction("set_font", &SoulListMenu::set_font)
        .endClass()
        .deriveClass<SoulVertMenu, SoulListMenu>("SoulVertMenu")
        .addConstructorFrom<std::shared_ptr<SoulVertMenu>, void(float, float, float, float, const std::string&, bool)>()
        .endClass()
        .deriveClass<SoulHorMenu, SoulListMenu>("SoulHorMenu")
        .addConstructorFrom<std::shared_ptr<SoulHorMenu>, void(float, float, float, float, const std::string&, bool)>()
        .endClass()
        .deriveClass<OptionMenu, EntityGroup>("OptionMenu")
        .addConstructorFrom<std::shared_ptr<OptionMenu>, void(float, float, float, float, const std::string&)>()
        .addFunction("add_option", &OptionMenu::add_option)
        .endClass()
        .deriveClass<KeyboardEntity, Entity>("KeyboardEntity")
        .addConstructorFrom<std::shared_ptr<KeyboardEntity>,
                            void(float, float, const std::string&, luabridge::LuaRef, luabridge::LuaRef, luabridge::LuaRef)>()
        .addFunction("typed_text", &KeyboardEntity::typed_text)
        .addFunction("set_typed_text", &KeyboardEntity::set_typed_text)
        .endClass()
        .deriveClass<SpriteEntity, Entity>("SpriteEntity")
        .addConstructorFrom<std::shared_ptr<SpriteEntity>, void(const std::string&, float, float, float)>()
        .addFunction("set_pos", &SpriteEntity::set_pos)
        .addFunction("x", &SpriteEntity::x)
        .addFunction("y", &SpriteEntity::y)
        .addFunction("set_tint", &SpriteEntity::set_tint)
        .endClass()
        .deriveClass<Soul, SpriteEntity>("Soul")
        .addConstructorFrom<std::shared_ptr<Soul>, void(float, float, const std::string&, float)>()
        .endClass()
        .deriveClass<MenuSoul, Soul>("MenuSoul")
        .addConstructorFrom<std::shared_ptr<MenuSoul>, void(float, float, bool, bool)>()
        .addFunction("add_snap_point", &MenuSoul::add_snap_point)
        .addFunction("get_selected", &MenuSoul::get_selected)
        .endClass()
        .deriveClass<ImageEntity, Entity>("ImageEntity")
        .addConstructorFrom<std::shared_ptr<ImageEntity>,
                            void(const std::string&, float, float, float, float, float, float)>()
        .addFunction("start_fade_out", &ImageEntity::start_fade_out)
        .addFunction("is_done", &ImageEntity::is_done)
        .addFunction("set_clip", &ImageEntity::set_clip)
        .addFunction("set_pan_delay", &ImageEntity::set_pan_delay)
        .endClass()
        .deriveClass<TextEntity, Entity>("TextEntity")
        .addConstructorFrom<std::shared_ptr<TextEntity>,
                            void(const std::string&, float, float, const std::string&, const std::string&, float, int)>()
        .addFunction("is_finished", &TextEntity::is_finished)
        .addFunction("skip", &TextEntity::skip)
        .addFunction("set_font_size", &TextEntity::set_font_size)
        .addFunction("set_font", &TextEntity::set_font)
        .addFunction("set_centered", &TextEntity::set_centered)
        .addFunction("set_spacing", &TextEntity::set_spacing)
        .addFunction("set_pos", &TextEntity::set_pos)
        .addFunction("set_color", &TextEntity::set_color)
        .addFunction("set_text", &TextEntity::set_text)
        .endClass();
}
