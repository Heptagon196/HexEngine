set_languages("c++20")
add_requires("libsdl", "libsdl_image", "libsdl_mixer", "libsdl_ttf", "box2d")
add_rules("mode.release")
includes("core")
target("hex")
    set_kind("binary")
    add_deps("hexcore")
    add_files("*.cpp")
