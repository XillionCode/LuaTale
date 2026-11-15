#include "LuaBindings.hpp"
#include <cstdint>
#include "entity/BattleSoul.hpp"
#include "entity/BoxEntity.hpp"
#include "entity/DialogueEntity.hpp"
#include "entity/Entity.hpp"
#include "entity/NPCEntity.hpp"
#include "entity/EntityGroup.hpp"
#include "entity/FadeOverlayEntity.hpp"
#include "entity/ImageEntity.hpp"
#include "menu/KeyboardEntity.hpp"
#include "entity/MenuSoul.hpp"
#include "menu/OptionMenu.hpp"
#include "entity/OverworldFrisk.hpp"
#include "entity/PlayerEntity.hpp"
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
        .addFunction("set_visible", &Entity::set_visible)
        .addFunction("visible", &Entity::visible)
        .addFunction("set_active", &Entity::set_active)
        .addFunction("active", &Entity::active)
        .addFunction("depth", &Entity::depth)
        .addFunction("set_depth", &Entity::set_depth)
        .endClass()
        .deriveClass<BoxEntity, Entity>("BoxEntity")
        .addConstructorFrom<std::shared_ptr<BoxEntity>, void(float, float, float, float, float)>()
        .addFunction("set_pos", &BoxEntity::set_pos)
        .addFunction("set_size", &BoxEntity::set_size)
        .addFunction("set_border_color", &BoxEntity::set_border_color)
        .addFunction("set_fill_color", &BoxEntity::set_fill_color)
        .endClass()
        .deriveClass<EntityGroup, Entity>("EntityGroup")
        .addConstructorFrom<std::shared_ptr<EntityGroup>, void()>()
        .addFunction("add_child", &EntityGroup::add_child)
        .addFunction("clear_children", &EntityGroup::clear_children)
        .endClass()
        .deriveClass<SoulListMenu, EntityGroup>("SoulListMenu")
        .addFunction("add_option", &SoulListMenu::add_option)
        .addFunction("clear_options", &SoulListMenu::clear_options)
        .addFunction("set_option_label", &SoulListMenu::set_option_label)
        .addFunction("set_option_color", &SoulListMenu::set_option_color)
        .addFunction("set_selected", &SoulListMenu::set_selected)
        .addFunction("selected", &SoulListMenu::selected)
        .addFunction("on_move", &SoulListMenu::on_move)
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
        .addFunction("set_texture", &SpriteEntity::set_texture)
        .addFunction("x", &SpriteEntity::x)
        .addFunction("y", &SpriteEntity::y)
        .addFunction("width", &SpriteEntity::width)
        .addFunction("height", &SpriteEntity::height)
        .addFunction("set_tint", &SpriteEntity::set_tint)
        .endClass()
        .deriveClass<NPCEntity, SpriteEntity>("NPCEntity")
        .addConstructorFrom<std::shared_ptr<NPCEntity>, void(const std::string&, float, float, float)>()
        .addFunction("add_frame", &NPCEntity::add_frame)
        .addFunction("set_frame", &NPCEntity::set_frame)
        .addFunction("set_anim_speed", &NPCEntity::set_anim_speed)
        .addFunction("set_solid", &NPCEntity::set_solid)
        .addFunction("solid", &NPCEntity::solid)
        .addFunction("on_interact", &NPCEntity::on_interact)
        .addFunction("walk_to", &NPCEntity::walk_to)
        .endClass()
        .deriveClass<DialogueEntity, EntityGroup>("DialogueEntity")
        .addConstructorFrom<std::shared_ptr<DialogueEntity>,
                            void(bool, bool, const std::string&, const std::string&, float, int32_t)>()
        .addFunction("set_text", &DialogueEntity::set_text)
        .addFunction("add_page", &DialogueEntity::add_page)
        .addFunction("set_has_portrait", &DialogueEntity::set_has_portrait)
        .addFunction("set_top_side", &DialogueEntity::set_top_side)
        .addFunction("set_portrait", &DialogueEntity::set_portrait)
        .addFunction("on_emote", &DialogueEntity::on_emote)
        .addFunction("on_mouth_change", &DialogueEntity::on_mouth_change)
        .addFunction("on_done", &DialogueEntity::on_done)
        .addFunction("is_done", &DialogueEntity::is_done)
        .endClass()
        .deriveClass<Soul, SpriteEntity>("Soul")
        .addConstructorFrom<std::shared_ptr<Soul>, void(float, float, const std::string&, float)>()
        .endClass()
        .deriveClass<MenuSoul, Soul>("MenuSoul")
        .addConstructorFrom<std::shared_ptr<MenuSoul>, void(float, float, bool, bool)>()
        .addFunction("add_snap_point", &MenuSoul::add_snap_point)
        .addFunction("clear_snap_points", &MenuSoul::clear_snap_points)
        .addFunction("set_selected", &MenuSoul::set_selected)
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
                            void(const std::string&, float, float, const std::string&, const std::string&, float, int32_t)>()
        .addFunction("is_finished", &TextEntity::is_finished)
        .addFunction("is_typing", &TextEntity::is_typing)
        .addFunction("current_emote", &TextEntity::current_emote)
        .addFunction("skip", &TextEntity::skip)
        .addFunction("restart", &TextEntity::restart)
        .addFunction("on_emote", &TextEntity::on_emote)
        .addFunction("set_font_size", &TextEntity::set_font_size)
        .addFunction("set_font", &TextEntity::set_font)
        .addFunction("set_centered", &TextEntity::set_centered)
        .addFunction("set_spacing", &TextEntity::set_spacing)
        .addFunction("set_pos", &TextEntity::set_pos)
        .addFunction("set_rotation", &TextEntity::set_rotation)
        .addFunction("set_color", &TextEntity::set_color)
        .addFunction("set_text", &TextEntity::set_text)
        .endClass()
        .deriveClass<FadeOverlayEntity, Entity>("FadeOverlayEntity")
        .addConstructorFrom<std::shared_ptr<FadeOverlayEntity>,
                            void(unsigned char, unsigned char, unsigned char, float)>()
        .endClass()
        .deriveClass<PlayerEntity, Entity>("PlayerEntity")
        .addFunction("set_pos", &PlayerEntity::set_pos)
        .addFunction("set_bounds", &PlayerEntity::set_bounds)
        .addFunction("set_debug", &PlayerEntity::set_debug)
        .addFunction("set_frozen", &PlayerEntity::set_frozen)
        .addFunction("frozen", &PlayerEntity::frozen)
        .addFunction("x", &PlayerEntity::x)
        .addFunction("y", &PlayerEntity::y)
        .endClass()
        .deriveClass<OverworldFrisk, PlayerEntity>("OverworldFrisk")
        .addConstructorFrom<std::shared_ptr<OverworldFrisk>, void(float, float)>()
        .endClass()
        .deriveClass<BattleSoul, PlayerEntity>("BattleSoul")
        .addConstructorFrom<std::shared_ptr<BattleSoul>, void(float, float)>()
        .endClass();
}
