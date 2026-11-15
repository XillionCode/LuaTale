local overworld_menu = require("scripts.overworld_menu")
local player_state = require("scripts.player_state")

local M = {}

local RUINS_ORDER = {
	"room_area1",
	"room_area1_2",
	"room_ruins1",
	"room_ruins2",
	"room_ruins3",
	"room_ruins4",
	"room_ruins5",
	"room_ruins6",
	"room_ruins7",
	"room_ruins7A",
	"room_ruins8",
	"room_ruins9",
	"room_ruins10",
	"room_ruins11",
	"room_ruins12A",
	"room_ruins12",
	"room_ruins12B",
	"room_ruins13",
	"room_ruins14",
	"room_ruins15A",
	"room_ruins15B",
	"room_ruins15C",
	"room_ruins15D",
	"room_ruins15E",
	"room_ruins16",
	"room_ruins17",
	"room_ruins18OLD",
	"room_ruins19",
	"room_torhouse1",
	"room_torhouse2",
	"room_torhouse3",
	"room_torielroom",
	"room_asrielroom",
	"room_basement1",
	"room_basement2",
	"room_basement3",
	"room_basement4",
	"room_basement5",
	"room_ruinsexit",
}

local ROOM_INDEX = {}
for i, name in ipairs(RUINS_ORDER) do
	ROOM_INDEX[name] = i
end

local SPECIAL_DOORS = {
	room_ruins12   = { obj_door_ruins13 = { room = "room_ruins13",   marker = "obj_markerX" } },
	room_ruins13   = { obj_door_ruins13 = { room = "room_ruins12",   marker = "obj_markerX" } },
	room_ruins15E  = { obj_door_ruins13 = { room = "room_ruins15A",  marker = "obj_markerX" } },
	room_ruins16   = { obj_door_ruins13 = { room = "room_ruins19",   marker = "obj_markerX" } },
	room_ruins19   = { obj_door_ruins13 = { room = "room_ruins16",   marker = "obj_markerX" } },
	room_torhouse1 = { obj_door_ruins13 = { room = "room_basement1", marker = "obj_markerX" } },
	room_basement1 = { obj_door_ruins13 = { room = "room_torhouse1", marker = "obj_markerX" } },
	room_torhouse2 = { obj_door_ruins13 = { room = "room_kitchen",   marker = "obj_markerX" } },
	room_kitchen   = { obj_door_ruins13 = { room = "room_torhouse2", marker = "obj_markerX" } },
	room_basement4 = { obj_finaldoor    = { room = "room_basement5", marker = "obj_markerA" } },
	room_ruinsexit = { obj_demodoorA    = { room = "room_area1",     marker = "" } },
}

local function resolve_door(room_name, door_name)
	local special = SPECIAL_DOORS[room_name]
	if special and special[door_name] then
		return special[door_name].room, special[door_name].marker
	end

	local idx = ROOM_INDEX[room_name]
	if not idx then
		return nil, nil
	end

	if door_name == "obj_doorA" or door_name == "obj_doorAmusicfade" then
		return RUINS_ORDER[idx + 1], "obj_markerA"
	elseif door_name == "obj_doorB" or door_name == "obj_doorBmusicfade" then
		return RUINS_ORDER[idx - 1], "obj_markerB"
	elseif door_name == "obj_doorC" then
		return RUINS_ORDER[idx + 2], "obj_markerC"
	elseif door_name == "obj_doorD" then
		return RUINS_ORDER[idx - 2], "obj_markerD"
	end
	return nil, nil
end

local function update_music(room_name)
	if room_name == "room_area1" or room_name == "room_area1_2" then
		music:stop()
	elseif room_name:match("^room_torhouse") or
	       room_name == "room_torielroom" or
	       room_name == "room_asrielroom" or
	       room_name == "room_kitchen" then
		music:start_song("ref/mus_ruinspiano.ogg")
		music:set_volume(1.0)
	else
		music:start_song("ref/mus_ruins.ogg")
		music:set_volume(1.0)
	end
end

function M.enter(room_name, marker_name)
	update_music(room_name)
	local room = OverworldRoom("assets/rooms/" .. room_name .. ".json")
	local x = (marker_name and marker_name ~= "") and room:marker_x(marker_name) or room:spawn_x()
	local y = (marker_name and marker_name ~= "") and room:marker_y(marker_name) or room:spawn_y()

	local frisk = OverworldFrisk(x, y)
	frisk:set_debug(DEBUG_MODE)
	room:add_entity(frisk)
	room:set_follow(frisk)
	overworld_menu.attach(room, frisk)

	if room_name == "room_area1_2" and player_state.plot == 0 then
		local fx = room:marker_x("obj_floweytalker1")
		local fy = room:marker_y("obj_floweytalker1")
		local flowey = NPCEntity("assets/Sprites/spr_flowey_0.png", fx, fy, 1.0)
		flowey:add_frame("assets/Sprites/spr_flowey_1.png")
		flowey:set_solid(true)
		room:add_entity(flowey)

		room:on_trigger(function(trig_name)
			if trig_name == "obj_floweytrigger" and player_state.plot == 0 then
				frisk:set_frozen(true)
				music:start_song("ref/mus_flowey.ogg")
				music:set_volume(1.0)

				local font = engine:language() == "ja" and "fnt_ja_maintext" or "fnt_maintext"
				local top_side = frisk:y() > room:view_y() + 130
				local dlg = DialogueEntity(top_side, true, font, "assets/embedded_audio/audio_97.wav", 32, 1)
				dlg:set_portrait(0, "assets/Sprites/spr_floweynice_0.png", "assets/Sprites/spr_floweynice_1.png")
				dlg:set_portrait(1, "assets/Sprites/spr_floweynicesideum_0.png", "assets/Sprites/spr_floweynicesideum_1.png")
				dlg:set_portrait(2, "assets/Sprites/spr_floweysassy_0.png", "assets/Sprites/spr_floweysassy_1.png")
				dlg:set_portrait(3, "assets/Sprites/spr_floweypissed_0.png", "assets/Sprites/spr_floweypissed_1.png")
				dlg:set_portrait(4, "assets/Sprites/spr_floweyevil_0.png", "assets/Sprites/spr_floweyevil_1.png")
				dlg:set_portrait(5, "assets/Sprites/spr_floweygrin_0.png", "assets/Sprites/spr_floweygrin_1.png")

				local text = ""
				for i = 291, 297 do
					text = text .. engine:get_text("SCR_TEXT_" .. i)
				end
				dlg:set_text(text)
				dlg:on_done(function()
					player_state.plot = 1
					frisk:set_frozen(false)
					update_music(room_name)
				end)
				room:add_ui_entity(dlg)
			end
		end)
	end
	room:on_door(function(door_name)
		local next_room, next_marker = resolve_door(room_name, door_name)
		if next_room then
			M.enter(next_room, next_marker)
		end
	end)

	engine:set_room(room)
end

return M
