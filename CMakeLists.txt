cmake_minimum_required(VERSION 3.5)
project(genland)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -fopenmp -lGL -lGLU")

set(SOURCE_FILES bin/Main.cpp bin/SoundManager.cpp bin/SoundManager.hpp bin/scene/entities/AnimatedTile.cpp bin/MagicView.cpp bin/scene/entities/AnimatedTile.h bin/scene/world/Chunk.cpp bin/scene/world/Chunk.h bin/Game.cpp bin/Game.h bin/Inputs.cpp bin/Inputs.h bin/scene/Inventory.cpp bin/scene/Inventory.h bin/scene/entities/Item.cpp bin/scene/entities/Item.h bin/scene/world/Map.cpp bin/scene/world/Map.h bin/scene/Player.cpp bin/scene/Player.h bin/scene/world/Tile.cpp bin/scene/world/Tile.h bin/scene/world/Simplex2d.cpp bin/scene/world/Simplex2d.h bin/TextureManager.cpp bin/TextureManager.h bin/scene/world/Background.cpp bin/scene/world/Background.h bin/scene/entities/Light.cpp bin/scene/entities/Light.h bin/scene/Drawer.cpp bin/scene/Drawer.h bin/scene/world/WorldBackground.cpp bin/scene/world/WorldBackground.h bin/scene/Clock.cpp bin/scene/Clock.h bin/menu_interface/MenuMain.cpp bin/menu_interface/MenuMain.h bin/menu_interface/Button.cpp bin/menu_interface/Button.h bin/menu_interface/NewGameMenu.cpp bin/menu_interface/NewGameMenu.h bin/scene/Scene.cpp bin/scene/Scene.h bin/menu_interface/MenuLoadGame.cpp bin/menu_interface/MenuLoadGame.h bin/menu_interface/MenuConfigGame.cpp bin/menu_interface/MenuConfigGame.h bin/menu_interface/TextInput.cpp bin/menu_interface/TextInput.h bin/menu_interface/InterfaceList.cpp bin/menu_interface/InterfaceList.h bin/Settings.h bin/ConfigStarter.cpp bin/ConfigStarter.h bin/functions.h bin/Debuger.cpp bin/Debuger.h bin/Resources.cpp bin/Resources.h bin/scene/world/MapView.cpp bin/scene/world/MapView.h bin/scene/entities/Tree.cpp bin/scene/entities/Tree.h bin/scene/entities/Entity.cpp bin/scene/entities/Entity.h bin/scene/entities/TreeGenetics.cpp bin/scene/entities/TreeGenetics.h bin/scene/NoiseGenerator.cpp bin/scene/NoiseGenerator.h bin/scene/world/Ecosystem.cpp bin/scene/world/Ecosystem.h bin/Utils.h bin/Terminal.cpp bin/Terminal.h bin/scene/entities/Torch.cpp bin/scene/entities/Torch.h bin/scene/entities/Stairs.cpp bin/scene/entities/Stairs.h bin/scene/entities/Mob.cpp bin/scene/entities/Mob.h bin/scene/entities/MobGenetics.cpp bin/scene/entities/MobGenetics.h bin/scene/Colisionable.cpp bin/scene/Colisionable.h bin/scene/entities/MobModule.cpp bin/scene/entities/MobModule.h bin/Mouse.cpp bin/Mouse.h)
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