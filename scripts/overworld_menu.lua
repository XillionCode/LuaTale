local state = require("scripts.player_state")

local M = {}

local function active_font()
	return engine:language() == "ja" and "fnt_ja_maintext" or "fnt_maintext"
end

local function fmt_stat(key, a1, a2)
	local s = engine:get_text(key)
	s = s:gsub("\\?%[C%]", state.charname)
	s = s:gsub("\\?%[G%]", tostring(state.gold))
	s = s:gsub("\\?%[1%]", tostring(a1 or ""))
	s = s:gsub("\\?%[2%]", tostring(a2 or ""))
	s = s:gsub("#", "\n")
	return s
end

local function make_text(str, x, y, font, size)
	local t = TextEntity(str, x, y, font, "", size or 32, 1)
	t:skip()
	return t
end

function M.attach(room, frisk)
	local font = active_font()
	local menuno = -1
	local selected_slot = 1

	-- Left HUD: Status Box + Command Box
	local hud_group = EntityGroup()
	hud_group:set_visible(false)
	hud_group:set_active(false)

	local status_box = BoxEntity(32, 52, 140, 108, 6)
	local status_name = make_text(state.charname, 46, 60, font, 32)
	local status_lv_lbl = make_text("LV", 46, 100, "fnt_small", 32)
	local status_lv_val = make_text(tostring(state.lv), 86, 100, "fnt_small", 32)
	local status_hp_lbl = make_text("HP", 46, 118, "fnt_small", 32)
	local status_hp_val = make_text(state.hp .. "/" .. state.maxhp, 86, 118, "fnt_small", 32)
	local status_g_lbl = make_text("G", 46, 136, "fnt_small", 32)
	local status_g_val = make_text(tostring(state.gold), 86, 136, "fnt_small", 32)

	hud_group:add_child(status_box)
	hud_group:add_child(status_name)
	hud_group:add_child(status_lv_lbl)
	hud_group:add_child(status_lv_val)
	hud_group:add_child(status_hp_lbl)
	hud_group:add_child(status_hp_val)
	hud_group:add_child(status_g_lbl)
	hud_group:add_child(status_g_val)

	local cmd_box = BoxEntity(32, 168, 140, 146, 6)
	hud_group:add_child(cmd_box)

	local cmd_menu = SoulVertMenu(84, 188, 36, -28, font, false)
	cmd_menu:on_move(function()
		music:play_sfx("assets/embedded_audio/audio_114.wav")
	end)
	hud_group:add_child(cmd_menu)
	room:add_ui_entity(hud_group)

	-- Right Submenu 1/5: ITEM list + USE/INFO/DROP bar
	local item_group = EntityGroup()
	item_group:set_visible(false)
	item_group:set_active(false)

	local item_box = BoxEntity(188, 52, 344, 360, 6)
	item_group:add_child(item_box)

	local item_vert = SoulVertMenu(232, 80, 32, -28, font, false)
	item_vert:on_move(function()
		music:play_sfx("assets/embedded_audio/audio_114.wav")
	end)
	item_group:add_child(item_vert)

	local action_hor = SoulHorMenu(232, 360, 104, -28, font, false)
	action_hor:on_move(function()
		music:play_sfx("assets/embedded_audio/audio_114.wav")
	end)
	item_group:add_child(action_hor)
	room:add_ui_entity(item_group)

	-- Right Submenu 2: STAT screen
	local stat_group = EntityGroup()
	stat_group:set_visible(false)
	stat_group:set_active(false)

	local stat_box = BoxEntity(188, 52, 344, 416, 6)
	local stat_name = make_text("", 216, 84, font, 32)
	local stat_namehack = make_text(fmt_stat("stat_menu_namehack"), 384, 84, font, 24)
	local stat_lv = make_text("", 216, 144, font, 32)
	local stat_hp = make_text("", 216, 176, font, 32)
	local stat_at = make_text("", 216, 240, font, 32)
	local stat_df = make_text("", 216, 272, font, 32)
	local stat_exp = make_text("", 384, 240, font, 32)
	local stat_next = make_text("", 384, 272, font, 32)
	local stat_weapon = make_text("", 216, 332, font, 32)
	local stat_armor = make_text("", 216, 364, font, 32)
	local stat_gold = make_text("", 216, 404, font, 32)
	local stat_kills = make_text("", 384, 404, font, 32)

	stat_group:add_child(stat_box)
	stat_group:add_child(stat_name)
	stat_group:add_child(stat_namehack)
	stat_group:add_child(stat_lv)
	stat_group:add_child(stat_hp)
	stat_group:add_child(stat_at)
	stat_group:add_child(stat_df)
	stat_group:add_child(stat_exp)
	stat_group:add_child(stat_next)
	stat_group:add_child(stat_weapon)
	stat_group:add_child(stat_armor)
	stat_group:add_child(stat_gold)
	stat_group:add_child(stat_kills)
	room:add_ui_entity(stat_group)

	local function refresh_status()
		local offset_y = (frisk:y() > room:view_y() + 130) and 270 or 0
		status_box:set_pos(32, 52 + offset_y)
		status_name:set_pos(46, 60 + offset_y)
		status_name:set_text(state.charname)
		status_lv_lbl:set_pos(46, 100 + offset_y)
		status_lv_val:set_pos(86, 100 + offset_y)
		status_lv_val:set_text(tostring(state.lv))
		status_hp_lbl:set_pos(46, 118 + offset_y)
		status_hp_val:set_pos(86, 118 + offset_y)
		status_hp_val:set_text(state.hp .. "/" .. state.maxhp)
		status_g_lbl:set_pos(46, 136 + offset_y)
		status_g_val:set_pos(86, 136 + offset_y)
		status_g_val:set_text(tostring(state.gold))

		for _, t in ipairs({ status_name, status_lv_val, status_hp_val, status_g_val }) do
			t:skip()
		end
	end

	local function refresh_stat_screen()
		stat_name:set_text(fmt_stat("stat_menu_name"))
		stat_namehack:set_visible(#state.charname >= 7)
		stat_lv:set_text(fmt_stat("stat_menu_lv", state.lv))
		stat_hp:set_text(fmt_stat("stat_menu_hp", state.hp, state.maxhp))
		stat_at:set_text(fmt_stat("stat_menu_at", state.at - 10, state.wstrength))
		stat_df:set_text(fmt_stat("stat_menu_df", state.df - 10, state.adef))
		stat_exp:set_text(fmt_stat("stat_menu_exp", state.xp))
		stat_next:set_text(fmt_stat("stat_menu_next", state.next_level_xp()))
		stat_weapon:set_text(fmt_stat("stat_menu_weapon", state.item_name(state.weapon)))
		stat_armor:set_text(fmt_stat("stat_menu_armor", state.armor_stat_name()))
		stat_gold:set_text(fmt_stat("stat_menu_gold"))
		stat_kills:set_visible(state.kills > 20)
		if state.kills > 20 then
			stat_kills:set_text(fmt_stat("stat_menu_kills", state.kills))
		end
		for _, t in ipairs({ stat_name, stat_lv, stat_hp, stat_at, stat_df, stat_exp, stat_next, stat_weapon, stat_armor, stat_gold, stat_kills }) do
			t:skip()
		end
	end

	local set_menu_state

	local function refresh_items()
		item_vert:clear_options()
		local count = state.item_count()
		for i = 1, count do
			local slot = i
			item_vert:add_option(state.item_name(state.items[slot]), function()
				selected_slot = slot
				music:play_sfx("assets/embedded_audio/audio_111.wav")
				set_menu_state(5)
			end)
		end
		if selected_slot > count then
			selected_slot = math.max(1, count)
		end
		item_vert:set_selected(selected_slot - 1)

		action_hor:clear_options()
		action_hor:add_option(engine:get_text("item_menu_use"), function()
			state.use_item(selected_slot)
			refresh_status()
			if state.item_count() == 0 then
				set_menu_state(0)
			else
				refresh_items()
				set_menu_state(1)
			end
		end)
		action_hor:add_option(engine:get_text("item_menu_info"), function()
			music:play_sfx("assets/embedded_audio/audio_111.wav")
			set_menu_state(1)
		end)
		action_hor:add_option(engine:get_text("item_menu_drop"), function()
			state.item_shift(selected_slot)
			music:play_sfx("assets/embedded_audio/audio_114.wav")
			if state.item_count() == 0 then
				set_menu_state(0)
			else
				refresh_items()
				set_menu_state(1)
			end
		end)
	end

	local function rebuild_cmd_menu()
		local prev_idx = cmd_menu:selected()
		cmd_menu:clear_options()
		cmd_menu:add_option(engine:get_text("field_menu_item"), function()
			if state.item_count() > 0 then
				selected_slot = 1
				music:play_sfx("assets/embedded_audio/audio_111.wav")
				refresh_items()
				set_menu_state(1)
			end
		end)
		if state.item_count() == 0 then
			cmd_menu:set_option_color(0, 128, 128, 128, 255)
		end
		cmd_menu:add_option(engine:get_text("field_menu_stat"), function()
			music:play_sfx("assets/embedded_audio/audio_111.wav")
			refresh_stat_screen()
			set_menu_state(2)
		end)
		if state.menuchoice[3] then
			cmd_menu:add_option(engine:get_text("field_menu_cell"), function()
				music:play_sfx("assets/embedded_audio/audio_111.wav")
			end)
		end
		cmd_menu:set_selected(prev_idx)
	end

	set_menu_state = function(next_state)
		menuno = next_state
		if menuno == -1 then
			frisk:set_frozen(false)
			hud_group:set_visible(false)
			hud_group:set_active(false)
			item_group:set_visible(false)
			item_group:set_active(false)
			stat_group:set_visible(false)
			stat_group:set_active(false)
			return
		end

		frisk:set_frozen(true)
		refresh_status()
		hud_group:set_visible(true)
		hud_group:set_active(menuno == 0)
		cmd_menu:set_active(menuno == 0)
		if menuno == 0 then
			rebuild_cmd_menu()
		end

		item_group:set_visible(menuno == 1 or menuno == 5)
		item_group:set_active(menuno == 1 or menuno == 5)
		item_vert:set_active(menuno == 1)
		action_hor:set_active(menuno == 5)
		if menuno == 5 then
			action_hor:set_selected(0)
		end

		stat_group:set_visible(menuno == 2)
		stat_group:set_active(menuno == 2)
	end

	room:set_key_event("c", function()
		if menuno == -1 then
			music:play_sfx("assets/embedded_audio/audio_114.wav")
			set_menu_state(0)
		elseif menuno == 0 then
			set_menu_state(-1)
		end
	end)

	room:set_key_event("x", function()
		if menuno == 0 then
			set_menu_state(-1)
		elseif menuno == 1 or menuno == 2 or menuno == 3 then
			set_menu_state(0)
		elseif menuno == 5 then
			set_menu_state(1)
		end
	end)
end

return M
