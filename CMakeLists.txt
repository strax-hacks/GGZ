cmake_minimum_required(VERSION 3.16)
project(cardputerzero-openggs VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options
option(BUILD_DESKTOP_SIMULATOR "Build desktop simulator target" ON)
option(BUILD_TESTS "Build validation and smoke tests" ON)
set(TARGET_DEVICE "DESKTOP" CACHE STRING "Target device: DESKTOP or CARDPUTER_ZERO")

# Compile definitions
if(TARGET_DEVICE STREQUAL "CARDPUTER_ZERO")
    add_compile_definitions(TARGET_CARDPUTER_ZERO=1)
else()
    add_compile_definitions(TARGET_DESKTOP=1)
endif()

# Asset install and runtime directory
if(NOT DEFINED DATADIR)
    set(DATADIR "/usr/share/openggs/data")
endif()
add_compile_definitions(OPENGGS_DATADIR="${DATADIR}")

# Find SDL2 and companion libraries
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)

link_directories(
    /opt/homebrew/lib
    ${SDL2_LIBRARY_DIRS}
    ${SDL2_IMAGE_LIBRARY_DIRS}
    ${SDL2_MIXER_LIBRARY_DIRS}
)

include_directories(
    src
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
)

# Core game sources
set(OPENGGS_SOURCES
    src/ANIMATION.cpp
    src/AUDIO.cpp
    src/CONTENT_Gametype_Load.cpp
    src/CONTENT_Intro_Stage0.cpp
    src/CONTENT_Stages_Load_Write.cpp
    src/ENEMIES.cpp
    src/ENEMIES_Collision.cpp
    src/ENEMIES_Draw.cpp
    src/ENEMIES_Move.cpp
    src/GAME_ENVIRONMENT.cpp
    src/GRAPHICS_Sprites.cpp
    src/GRAPHICS_Sprites_Bullets.cpp
    src/GRAPHICS_Sprites_Draw.cpp
    src/GRAPHICS_Sprites_Move.cpp
    src/GRAPHICS_Tiles_Animations.cpp
    src/GRAPHICS_Tiles_Define.cpp
    src/GRAPHICS_Tiles_Draw.cpp
    src/GRAPHICS_Tiles_Elements.cpp
    src/GRAPHICS_Tiles_Switch_Tile.cpp
    src/INTERFACE_Draw.cpp
    src/LOOP_Credits.cpp
    src/LOOP_Editor_Stages.cpp
    src/LOOP_Editor_Stages_Creator.cpp
    src/LOOP_Editor_Stages_Creator_Functions.cpp
    src/LOOP_Editor_Stages_Enemies.cpp
    src/LOOP_Editor_Stages_Interface.cpp
    src/LOOP_Editor_Stages_Set_Elements.cpp
    src/LOOP_Editor_Stages_Sprites.cpp
    src/LOOP_Editor_Stages_Stage_Settings.cpp
    src/LOOP_Editor_Tiles.cpp
    src/LOOP_Gameloop.cpp
    src/LOOP_Gameloop_Splashscreens.cpp
    src/LOOP_Intro.cpp
    src/LOOP_Menu.cpp
    src/LOOP_Options.cpp
    src/LOOP_Stagefile_Load.cpp
    src/LOOP_Story.cpp
    src/PLAYER.cpp
    src/PLAYER_Collision_NonSolids.cpp
    src/PLAYER_Collision_Solids.cpp
    src/PLAYER_Collision_Solids_Specials.cpp
    src/PLAYER_Collision_Sprites.cpp
    src/PLAYER_Draw.cpp
    src/PLAYER_Jump.cpp
    src/PLAYER_Killed.cpp
    src/PLAYER_Run.cpp
    src/STAGE_Draw.cpp
    src/STAGE_Interface_Draw.cpp
    src/SYSTEM_Debug_Infos.cpp
    src/SYSTEM_Draw_Text.cpp
    src/SYSTEM_Input_General.cpp
    src/SYSTEM_SDL_Textures.cpp
    src/SYSTEM_SDL_init.cpp
    src/SYSTEM_SDL_Close.cpp
    src/SYSTEM_SDL_input.cpp
    src/SYSTEM_Update_Screen.cpp
    src/SYSTEM_Vector_Operations.cpp
    src/SYSTEM_Paths.cpp
    src/SYSTEM_Power.cpp
    src/SYSTEM_SaveState.cpp
)

# Core game library
add_library(openggs_core STATIC
    ${OPENGGS_SOURCES}
)

target_include_directories(openggs_core PUBLIC
    src
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
)

target_link_libraries(openggs_core PUBLIC
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
)

# Main executable
add_executable(openggs-cz
    src/main.cpp
)

target_link_libraries(openggs-cz PRIVATE
    openggs_core
)

# Test targets
if(BUILD_TESTS)
    add_compile_definitions(OPENGGS_RENDER_STATS=1)
    enable_testing()
    add_subdirectory(tests)
endif()

# Installation rules for packaging
install(TARGETS openggs-cz DESTINATION /usr/share/openggs/bin)
install(DIRECTORY base/ DESTINATION /usr/share/openggs/data)
install(FILES packaging/openggs.desktop DESTINATION /usr/share/APPLaunch/applications)
install(FILES packaging/openggs.png DESTINATION /usr/share/APPLaunch/share/images)
