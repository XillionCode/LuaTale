local M = {}

local SPECIAL_ALLOW = {
	asgore = false,
	toriel = false,
	sans   = false,
	undyne = false,
	flowey = false,
	alphys = false,
	asriel = false,

	aaaaaa = true,
	chara  = true,
	alphy  = true,
	papyru = true,
	napsta = true,
	blooky = true,
	murder = true,
	mercy  = true,
	catty  = true,
	bratty = true,
	mtt    = true,
	metta  = true,
	mett   = true,
	gerson = true,
	shyren = true,
	aaron  = true,
	temmie = true,
	woshua = true,
	jerry  = true,
	bpants = true,
}

local function active_font()
	return engine:language() == "ja" and "fnt_ja_maintext" or "fnt_maintext"
end

local function format_text(key)
	return (engine:get_text(key):gsub("#", "\n"))
end

local function check_name(name)
	if name == "" then
		return false, format_text("name_entry_missing")
	end
	local lower = name:lower()
	if lower == "frisk" then
		return true, format_text("name_entry_hardmode")
	end
	local allow = SPECIAL_ALLOW[lower]
	if allow ~= nil then
		return allow, format_text("name_resp_" .. lower)
	end
	return true, format_text("name_entry_confirm")
end

function M.show_confirm(name, back_to_keyboard, on_accept)
	local room = Room()
	local font = active_font()
	local allow, msg = check_name(name)

	local prompt = TextEntity(msg, 180, 60, font, "", 32, 1)
	prompt:skip()
	room:add_entity(prompt)

	local preview = TextEntity(name, 280, 120, font, "", 32, 1)
	preview:skip()
	room:add_entity(preview)

	local choice = SoulHorMenu(160, 400, 320, -28, font, true)
	if allow then
		choice:add_option(format_text("no"), function()
			back_to_keyboard(name)
		end)
		choice:add_option(format_text("yes"), function()
			on_accept(name)
		end)
	else
		choice:add_option(format_text("name_entry_goback"), function()
			back_to_keyboard(name)
		end)
	end
	room:add_entity(choice)

	engine:set_room(room)
end

function M.start(initial_name, on_cancel, on_accept)
	local function show_keyboard(current_name)
		local room = Room()
		local font = active_font()

		local prompt = TextEntity(format_text("name_entry_title"), 180, 60, font, "", 32, 1)
		prompt:skip()
		room:add_entity(prompt)

		local preview = TextEntity(current_name, 280, 110, font, "", 32, 1)
		preview:skip()
		room:add_entity(preview)

		local kb = KeyboardEntity(
			110, 160, font,
			function(text)
				if text:lower() == "gaster" then
					engine:quit()
					return
				end
				preview:set_text(text)
				preview:skip()
			end,
			function()
				on_cancel()
			end,
			function(text)
				M.show_confirm(text, show_keyboard, on_accept)
			end
		)
		kb:set_typed_text(current_name)
		room:add_entity(kb)

		engine:set_room(room)
	end

	show_keyboard(initial_name or "")
end

return M
