cmake_minimum_required(VERSION 3.16)

add_executable(smoke_test
    smoke_test.cpp
    ../src/ANIMATION.cpp
    ../src/AUDIO.cpp
    ../src/CONTENT_Gametype_Load.cpp
    ../src/CONTENT_Stages_Load_Write.cpp
    ../src/ENEMIES.cpp
    ../src/ENEMIES_Collision.cpp
    ../src/ENEMIES_Draw.cpp
    ../src/ENEMIES_Move.cpp
    ../src/GAME_ENVIRONMENT.cpp
    ../src/GRAPHICS_Sprites.cpp
    ../src/GRAPHICS_Sprites_Bullets.cpp
    ../src/GRAPHICS_Sprites_Draw.cpp
    ../src/GRAPHICS_Sprites_Move.cpp
    ../src/GRAPHICS_Tiles_Animations.cpp
    ../src/GRAPHICS_Tiles_Define.cpp
    ../src/GRAPHICS_Tiles_Draw.cpp
    ../src/GRAPHICS_Tiles_Elements.cpp
    ../src/GRAPHICS_Tiles_Switch_Tile.cpp
    ../src/INTERFACE_Draw.cpp
    ../src/LOOP_Credits.cpp
    ../src/LOOP_Editor_Stages.cpp
    ../src/LOOP_Editor_Stages_Creator.cpp
    ../src/LOOP_Editor_Stages_Creator_Functions.cpp
    ../src/LOOP_Editor_Stages_Enemies.cpp
    ../src/LOOP_Editor_Stages_Interface.cpp
    ../src/LOOP_Editor_Stages_Set_Elements.cpp
    ../src/LOOP_Editor_Stages_Sprites.cpp
    ../src/LOOP_Editor_Stages_Stage_Settings.cpp
    ../src/LOOP_Editor_Tiles.cpp
    ../src/LOOP_Gameloop.cpp
    ../src/LOOP_Gameloop_Splashscreens.cpp
    ../src/LOOP_Menu.cpp
    ../src/LOOP_Options.cpp
    ../src/LOOP_Stagefile_Load.cpp
    ../src/LOOP_Story.cpp
    ../src/PLAYER.cpp
    ../src/PLAYER_Collision_NonSolids.cpp
    ../src/PLAYER_Collision_Solids.cpp
    ../src/PLAYER_Collision_Solids_Specials.cpp
    ../src/PLAYER_Collision_Sprites.cpp
    ../src/PLAYER_Draw.cpp
    ../src/PLAYER_Jump.cpp
    ../src/PLAYER_Killed.cpp
    ../src/PLAYER_Run.cpp
    ../src/STAGE_Draw.cpp
    ../src/STAGE_Interface_Draw.cpp
    ../src/SYSTEM_Debug_Infos.cpp
    ../src/SYSTEM_Draw_Text.cpp
    ../src/SYSTEM_Input_General.cpp
    ../src/SYSTEM_SDL_Textures.cpp
    ../src/SYSTEM_SDL_init.cpp
    ../src/SYSTEM_SDL_Close.cpp
    ../src/SYSTEM_SDL_input.cpp
    ../src/SYSTEM_Update_Screen.cpp
    ../src/SYSTEM_Vector_Operations.cpp
    ../src/SYSTEM_Paths.cpp
    ../src/SYSTEM_Power.cpp
    ../src/SYSTEM_SaveState.cpp
)

target_include_directories(smoke_test PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
)

target_link_directories(smoke_test PRIVATE
    /opt/homebrew/lib
    ${SDL2_LIBRARY_DIRS}
    ${SDL2_IMAGE_LIBRARY_DIRS}
    ${SDL2_MIXER_LIBRARY_DIRS}
)

target_link_libraries(smoke_test PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
)

add_test(NAME SmokeTest COMMAND smoke_test)
