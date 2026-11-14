local menu = require("scripts.menu")

local M = {}

local function go_to_title()
	music:stop()
	local title = RoomCutscene(
		"assets/Sprites/spr_undertaletitle_0.png",
		0,
		0,
		2.0,
		0,
		0,
		0.05,
		"",
		0,
		0,
		"",
		"",
		-1,
		menu.go_to_menu,
		nil
	)
	title:set_debug(DEBUG_MODE)
	engine:set_room(title)
end

local function intro_text(key)
	local s = engine:get_text(key)
	s = s:gsub("&", "\n")
	s = s:gsub("%s*\\E%d+.*$", "")
	s = s:gsub("%s*%%+$", "")
	return s
end

local panels = {
	{ img = "assets/Sprites/spr_introimage_0.png", key = "obj_introimage_70" },
	{ img = "assets/Sprites/spr_introimage_1.png", key = "obj_introimage_71" },
	{ img = "assets/Sprites/spr_introimage_2.png", key = "obj_introimage_72" },
	{ img = "assets/Sprites/spr_introimage_3.png", key = "obj_introimage_73" },
	{ img = "assets/Sprites/spr_introimage_4.png", key = "obj_introimage_74" },
	{ img = "assets/Sprites/spr_introimage_5.png", key = "obj_introimage_75" },
	{ img = "assets/Sprites/spr_introimage_6.png", key = "obj_introimage_76" },
	{ img = "assets/Sprites/spr_introimage_7.png", hold = 56 },
	{ img = "assets/Sprites/spr_introimage_8.png", hold = 64 },
	{ img = "assets/Sprites/spr_introimage_9.png", hold = 64 },
	{
		img = "assets/Sprites/spr_introlast_0.png",
		y = -420,
		pan_dy = 2.0,
		pan_delay = 60,
		hold = 260,
		clip = { 0, 60, 640, 220 },
	},
}

function M.play_panel(index)
	local p = panels[index]
	if not p then
		go_to_title()
		return
	end

	local room = RoomCutscene(
		p.img,
		p.x or 0,
		p.y or 0,
		p.scale or 2.0,
		p.pan_dx or 0,
		p.pan_dy or 0,
		0.05,
		p.key and intro_text(p.key) or "",
		p.text_x or 120,
		320,
		engine:language() == "ja" and "fnt_ja_maintext" or "fnt_maintext",
		"assets/embedded_audio/audio_94.wav",
		p.hold or 0,
		function()
			M.play_panel(index + 1)
		end,
		go_to_title
	)
	if room:text() then
		room:text():set_spacing(18, 36)
		if p.centered then
			room:text():set_centered(true)
		end
	end
	if p.clip then
		room:image():set_clip(p.clip[1], p.clip[2], p.clip[3], p.clip[4])
	end
	if p.pan_delay then
		room:image():set_pan_delay(p.pan_delay)
	end
	room:set_debug(DEBUG_MODE)
	engine:set_room(room)
end

return M
