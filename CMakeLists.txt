cmake_minimum_required(VERSION 3.5)
project(genland)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

set(SOURCE_FILES Main.cpp AnimatedTile.cpp AnimatedTile.h Chunk.cpp Chunk.h Game.cpp Game.h Inputs.cpp Inputs.h Inventory.cpp Inventory.h Item.cpp Item.h Map.cpp Map.h Player.cpp Player.h Tile.cpp Tile.h Simplex2d.cpp Simplex2d.h TextureManager.cpp TextureManager.h Background.cpp Background.h)
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