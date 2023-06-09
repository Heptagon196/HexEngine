set_languages("c++20")
add_rules("mode.release")
includes("Gos")
target("hexcore")
    set_kind("static")
    add_deps("gos")
    add_packages("libsdl", "libsdl_image", "libsdl_mixer", "libsdl_ttf", "box2d")
    add_files("Utils/*.cpp")
    add_files("Components/*.cpp")
    add_files("*.cpp")
