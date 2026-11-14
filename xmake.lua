set_project("under")
set_version("0.2.0")

set_languages("cxx20")
set_toolchains("clang", "gcc")

add_rules("mode.debug", "mode.release")

add_requires("luabridge3")
add_requires("yyjson")
add_requires("pkgconfig::raylib", {alias = "raylib"})
add_requires("pkgconfig::luajit", {alias = "luajit"})

target("under")
    set_kind("binary")
    set_rundir("$(projectdir)")
    add_includedirs("src")
    add_files("src/**.cpp")
    add_packages("luabridge3", "yyjson", "raylib", "luajit")
