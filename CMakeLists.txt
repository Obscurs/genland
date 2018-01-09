cmake_minimum_required(VERSION 3.5)
project(genland)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -fopenmp -lGL -lGLU")

set(SOURCE_FILES src/Main.cpp src/SoundManager.cpp src/SoundManager.hpp src/scene/entities/AnimatedTile.cpp src/MagicView.cpp src/scene/entities/AnimatedTile.h src/scene/world/Chunk.cpp src/scene/world/Chunk.h src/Game.cpp src/Game.h src/Inputs.cpp src/Inputs.h src/scene/Inventory.cpp src/scene/Inventory.h src/scene/entities/Item.cpp src/scene/entities/Item.h src/scene/world/Map.cpp src/scene/world/Map.h src/scene/Player.cpp src/scene/Player.h src/scene/world/Tile.cpp src/scene/world/Tile.h src/scene/world/Simplex2d.cpp src/scene/world/Simplex2d.h src/TextureManager.cpp src/TextureManager.h src/scene/world/Background.cpp src/scene/world/Background.h src/scene/entities/Light.cpp src/scene/entities/Light.h src/scene/Drawer.cpp src/scene/Drawer.h src/scene/world/WorldBackground.cpp src/scene/world/WorldBackground.h src/scene/Clock.cpp src/scene/Clock.h src/menu_interface/MenuMain.cpp src/menu_interface/MenuMain.h src/menu_interface/Button.cpp src/menu_interface/Button.h src/menu_interface/NewGameMenu.cpp src/menu_interface/NewGameMenu.h src/scene/Scene.cpp src/scene/Scene.h src/menu_interface/MenuLoadGame.cpp src/menu_interface/MenuLoadGame.h src/menu_interface/MenuConfigGame.cpp src/menu_interface/MenuConfigGame.h src/menu_interface/TextInput.cpp src/menu_interface/TextInput.h src/menu_interface/InterfaceList.cpp src/menu_interface/InterfaceList.h src/Settings.h src/ConfigStarter.cpp src/ConfigStarter.h src/functions.h src/Debuger.cpp src/Debuger.h src/Resources.cpp src/Resources.h src/scene/world/MapView.cpp src/scene/world/MapView.h src/scene/entities/Tree.cpp src/scene/entities/Tree.h src/scene/entities/Entity.cpp src/scene/entities/Entity.h src/scene/entities/TreeGenetics.cpp src/scene/entities/TreeGenetics.h src/scene/NoiseGenerator.cpp src/scene/NoiseGenerator.h src/scene/world/Ecosystem.cpp src/scene/world/Ecosystem.h src/Utils.h src/Terminal.cpp src/Terminal.h src/scene/entities/Torch.cpp src/scene/entities/Torch.h src/scene/entities/Stairs.cpp src/scene/entities/Stairs.h src/scene/entities/Mob.cpp src/scene/entities/Mob.h src/scene/entities/MobGenetics.cpp src/scene/entities/MobGenetics.h src/scene/Colisionable.cpp src/scene/Colisionable.h src/scene/entities/MobModule.cpp src/scene/entities/MobModule.h src/Mouse.cpp src/Mouse.h src/scene/entities/Picture.cpp src/scene/entities/Picture.h src/scene/entities/Table.cpp src/scene/entities/Table.h src/scene/entities/Chair.cpp src/scene/entities/Chair.h src/menu_interface/MenuAbout.cpp src/menu_interface/MenuAbout.h src/menu_interface/MenuHelp.cpp src/menu_interface/MenuHelp.h)
set(EXECUTABLE_NAME "genland")
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()