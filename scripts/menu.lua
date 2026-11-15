local naming = require("scripts.naming")
local player_state = require("scripts.player_state")
local ruins = require("scripts.ruins")

local M = {}

local function active_font()
	return engine:language() == "ja" and "fnt_ja_maintext" or "fnt_maintext"
end

function M.go_to_settings()
	local settings = Room()

	local title = TextEntity(engine:get_text("settings_title"), 320, 20, active_font(), "", 64, 1)
	title:set_centered(true)
	title:skip()
	settings:add_entity(title)

	local vert = SoulVertMenu(40, 80, 40, -28, active_font(), true)
	vert:add_option(engine:get_text("joyconfig_exit"), function()
		M.go_to_menu()
	end)
	local function lang_label()
		return engine:get_text("settings_language") .. " " .. engine:get_text("settings_language_" .. engine:language())
	end
	vert:add_option(lang_label(), function()
		if engine:language() == "en" then
			engine:set_language("ja")
		else
			engine:set_language("en")
		end
		local font = active_font()
		title:set_font(font)
		title:set_text(engine:get_text("settings_title"))
		vert:set_font(font)
		vert:set_option_label(0, engine:get_text("joyconfig_exit"))
		vert:set_option_label(1, lang_label())
	end)
	settings:add_entity(vert)

	engine:set_room(settings)
end

function M.go_to_menu()
	music:start_song("ref/mus_menu0.ogg")
	music:set_volume(0.6)
	music:set_pitch(1.0)
	local menu = Room()
	local font = active_font()

	local instr_text = table.concat({
		engine:get_text("instructions_title"),
		"",
		engine:get_text("instructions_confirm_key") .. " - " .. engine:get_text("instructions_confirm_label"),
		engine:get_text("instructions_cancel_key") .. " - " .. engine:get_text("instructions_cancel_label"),
		engine:get_text("instructions_menu_key") .. " - " .. engine:get_text("instructions_menu_label"),
		engine:get_text("instructions_fullscreen_key") .. " - " .. engine:get_text("instructions_fullscreen_label"),
		engine:get_text("instructions_quit_key") .. " - " .. engine:get_text("instructions_quit_label"),
		"",
		engine:get_text("instructions_hp0"),
	}, "\n")

	local instr = TextEntity(
		instr_text,
		170,
		40,
		font,
		"",
		32,
		1
	)
	instr:set_color(192, 192, 192, 255)
	instr:set_spacing(16, 32)
	instr:skip()
	menu:add_entity(instr)

	local vert = SoulVertMenu(230, 340, 40, -28, font, true)
	vert:add_option(engine:get_text("instructions_begin"), function()
		naming.start("", M.go_to_menu, function(name)
			player_state.charname = name
			music:stop()
			ruins.enter("room_area1")
		end)
	end)
	vert:add_option(engine:get_text("settings_name"), function()
		M.go_to_settings()
	end)
	menu:add_entity(vert)

	local footer = TextEntity("LuaTale v0.2.0 - UNDERTALE (C) Toby Fox 2015-2017", 320, 456, "fnt_small", "", 32, 1)
	footer:set_centered(true)
	footer:set_color(128, 128, 128, 255)
	footer:skip()
	menu:add_entity(footer)

	engine:set_room(menu)
end

return M
