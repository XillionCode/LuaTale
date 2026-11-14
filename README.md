# LuaTale

**LuaTale** is a fast, multiplatform, and moddable C++20 + LuaJIT source port / reimplementation of **Undertale**.

## Mission & Goals
- **Faithful feel:** Fixed 30 FPS step loop, crisp nearest-neighbor pixel scaling, and authentic timing/audio.
- **Clean OOP engine:** A modular C++20 core (`Engine`, `Room`, `Entity`, `SpriteEntity`, `TextEntity`, `MusicPlayer`) powered by **Raylib**.
- **Lua-first gameplay & modding:** All rooms, cutscenes, menus, dialogue, and battle logic live in **LuaJIT** scripts (via **LuaBridge3**), making the game easy to extend, hot-reload, and mod without repacking `data.win`.
- **Multiplatform:** Native builds on Linux, Windows, and macOS via **xmake**.

## Tech Stack
- **C++20** (Clang / GCC)
- **Raylib** (windowing, 2D rendering, input, audio)
- **LuaJIT** + **LuaBridge3** (scripting & C++/Lua bindings)
- **xmake** (build system)

## Build & Run
```bash
xmake
xmake run
```
