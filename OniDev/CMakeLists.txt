# cmake -G "MinGW Makefiles"
# mingw32-make -release

cmake_minimum_required(VERSION 2.8)

project(libonidev-game)

set(SRCS
    source/math/vec2.cpp
    source/game/object2d.cpp
    source/game/room.cpp
    source/collision/collision.cpp
    source/collision/convex_polygon.cpp
    source/collision/disk.cpp
    source/collision/line.cpp
    source/collision/mask.cpp
    source/collision/polygon.cpp
    source/collision/rect.cpp
    source/collision/tilemap.cpp
    )
    
include_directories("include")
add_definitions(-std=c++11)

add_library(onidev-game ${SRCS} ${HEADERS})
# target_compile_features(onidev_game PRIVATE cxx_variadic_templates)
