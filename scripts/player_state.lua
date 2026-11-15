local M = {
	charname = "CHARA",
	hp = 20,
	maxhp = 20,
	at = 10,
	df = 10,
	wstrength = 0,
	adef = 0,
	weapon = 3,
	armor = 4,
	gold = 0,
	xp = 0,
	lv = 1,
	kills = 0,
	items = { 0, 0, 0, 0, 0, 0, 0, 0 },
	phone = { 0, 0, 0, 0, 0, 0, 0, 0 },
	menuchoice = { true, true, false },
}

M.WEAPONS = {
	[3] = 0,   -- Stick
	[13] = 3,  -- Tough Glove
	[14] = 5,  -- Ballet Shoes
	[25] = 7,  -- Torn Notebook
	[45] = 2,  -- Toy Knife
	[47] = 10, -- Burnt Pan
	[49] = 12, -- Empty Gun
	[51] = 15, -- Worn Dagger
	[52] = 99, -- Real Knife
}

M.ARMORS = {
	[4] = 0,   -- Bandage
	[12] = 3,  -- Faded Ribbon
	[15] = 7,  -- Old Tutu
	[24] = 10, -- Cloudy Glasses
	[44] = 5,  -- Manly Bandanna
	[46] = 11, -- Stained Apron
	[48] = 12, -- Cowboy Hat (+5 ATK)
	[50] = 15, -- Heart Locket
	[53] = 99, -- The Locket
	[64] = 20, -- Temmie Armor (+10 ATK)
}

M.EXP_THRESHOLDS = {
	10, 30, 70, 120, 200, 300, 500, 800, 1200, 1700,
	2500, 3500, 5000, 7000, 10000, 15000, 25000, 50000, 99999,
}

M.HEAL_ITEMS = {
	[1] = 10,  -- Monster Candy
	[2] = 15,  -- Croquet Roll
	[5] = 15,  -- Rock Candy
	[6] = 12,  -- Pumpkin Rings
	[7] = 20,  -- Spider Donut
	[8] = 24,  -- Stoic Onion
	[9] = 17,  -- Ghost Fruit
	[10] = 24, -- Spider Cider
	[11] = 99, -- Butterscotch Pie
	[16] = 22, -- Snowman Piece
	[17] = 11, -- Nice Cream
	[18] = 12, -- Puppydough Icecream
	[19] = 11, -- Bisicle
	[20] = 11, -- Unisicle
	[21] = 22, -- Cinnamon Bunny
	[22] = 18, -- Astronaut Food
	[28] = 15, -- Crab Apple
	[29] = 11, -- Sea Tea
	[30] = 18, -- Abandoned Quiche
	[31] = 21, -- Temmie Flakes
	[32] = 20, -- Temmie Flakes (expensive)
	[33] = 2,  -- Temmie Flakes (on sale)
	[34] = 8,  -- Dog Salad (varies, base 8)
	[35] = 4,  -- Instant Noodles (overworld 4 / 15)
	[36] = 25, -- Hot Dog...?
	[37] = 25, -- Hot Cat
	[38] = 27, -- Glamburger
	[39] = 30, -- Legendary Hero
	[40] = 60, -- Steak in the Shape of Mettaton's Face
	[41] = 16, -- Popato Chisps
	[42] = 14, -- Junk Food
	[43] = 20, -- Starfait
	[59] = 15, -- Hush Puppy
	[60] = 19, -- Snail Pie
}

function M.recalc_equip()
	M.wstrength = M.WEAPONS[M.weapon] or 0
	M.adef = M.ARMORS[M.armor] or 0
	if M.armor == 48 then
		M.wstrength = M.wstrength + 5
	elseif M.armor == 64 then
		M.wstrength = M.wstrength + 10
	end
end

function M.equip_weapon(slot, item_id)
	if slot and slot >= 1 and slot <= 8 then
		M.items[slot] = M.weapon
	end
	M.weapon = item_id
	M.recalc_equip()
end

function M.equip_armor(slot, item_id)
	if slot and slot >= 1 and slot <= 8 then
		M.items[slot] = M.armor
	end
	M.armor = item_id
	M.recalc_equip()
end

function M.add_xp(amount)
	local prev_lv = M.lv
	M.xp = M.xp + (amount or 0)
	if M.xp >= 99999 then
		M.xp = 99999
		M.lv = 20
	else
		local new_lv = 1
		for i, req in ipairs(M.EXP_THRESHOLDS) do
			if M.xp >= req then
				new_lv = i + 1
			end
		end
		M.lv = new_lv
	end
	if M.lv ~= prev_lv then
		if M.lv >= 20 then
			M.lv = 20
			M.maxhp = 99
			M.at = 99
			M.df = 99
		else
			M.maxhp = 16 + (M.lv * 4)
			M.at = 8 + (M.lv * 2)
			M.df = 9 + math.ceil(M.lv / 4)
		end
		return true
	end
	return false
end

function M.next_level_xp()
	if M.lv >= 20 then
		return 0
	end
	return (M.EXP_THRESHOLDS[M.lv] or 0) - M.xp
end

function M.item_get(item_id)
	for i = 1, 8 do
		if M.items[i] == 0 then
			M.items[i] = item_id
			return true
		end
	end
	return false
end

function M.item_shift(slot)
	if not slot or slot < 1 or slot > 8 then
		return
	end
	for i = slot, 7 do
		M.items[i] = M.items[i + 1]
	end
	M.items[8] = 0
end

function M.item_count()
	local n = 0
	for i = 1, 8 do
		if M.items[i] ~= 0 then
			n = n + 1
		end
	end
	return n
end

function M.item_name(item_id)
	if not item_id or item_id == 0 then
		return ""
	end
	return engine:get_text("item_name_" .. tostring(item_id))
end

function M.armor_stat_name()
	if M.armor == 64 then
		return engine:get_text("stat_armor_temmie")
	end
	return M.item_name(M.armor)
end

function M.use_item(slot)
	local id = M.items[slot]
	if not id or id == 0 then
		return
	end
	if M.WEAPONS[id] ~= nil then
		M.equip_weapon(slot, id)
		music:play_sfx("assets/embedded_audio/audio_108.wav")
	elseif M.ARMORS[id] ~= nil then
		M.equip_armor(slot, id)
		music:play_sfx("assets/embedded_audio/audio_108.wav")
	elseif M.HEAL_ITEMS[id] ~= nil then
		M.hp = math.min(M.maxhp, M.hp + M.HEAL_ITEMS[id])
		M.item_shift(slot)
		music:play_sfx("assets/embedded_audio/audio_148.wav")
	end
end

M.recalc_equip()

return M
