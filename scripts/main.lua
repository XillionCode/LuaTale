DEBUG_MODE = true
engine:set_log_level("warn")

music:start_song("ref/mus_story.ogg")
music:set_pitch(0.91)

local intro = require("scripts.intro")
intro.play_panel(1)
