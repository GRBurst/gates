cmake_minimum_required (VERSION 2.8)

project (GATES)

LIST(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

option(USE_CLANG "build application with clang" OFF) # OFF is the default
if(USE_CLANG)
    SET(CMAKE_CXX_COMPILER  "/usr/bin/clang++")
endif(USE_CLANG)

add_definitions(-std=c++11 -O2 -Wall -Wextra -pedantic)

# OpenGL
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message(FATAL_ERROR "OpenGL not found!")
endif(NOT OPENGL_FOUND)
include_directories(${OPENGL_INCLUDE_DIR})
#target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

# GLFW
add_subdirectory(external/glfw-3.1.2)
include_directories(external/glfw-3.1.2/include)
#find_package(PkgConfig REQUIRED)
#pkg_search_module(GLFW REQUIRED glfw3)
#if(NOT GLFW_FOUND)
#    message(FATAL_ERROR "GLFW not found")
#endif(NOT GLFW_FOUND)
#include_directories(${GLFW_INCLUDE_DIRS})
#target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES})

# GLEW
set(GLEW external/glew-1.13.0)
find_path(GLEW glew.h)
find_library(GLEW GLEW_LIBRARIES)
include_directories(${GLEW})
#find_package(GLEW REQUIRED)
#if(NOT GLEW_FOUND)
#    message(FATAL_ERROR "GLEW not found!")
#endif(NOT GLEW_FOUND)
#include_directories(${GLEW_INCLUDE_DIRS})
#target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})

# FreeImage
set(FreeImage external/FreeImage)
find_path(FreeImage FreeImage.h)
find_library(FreeImage FreeImage_LIBRARIES)
include_directories(${FreeImage})
#include(cmake/FindFreeImage.cmake)
#find_package(FreeImage REQUIRED)
#if(NOT FreeImage_FOUND)
#    message(FATAL_ERROR "FreeImage not found!")
#endif(NOT FreeImage_FOUND)
#include_directories(${FreeImage_INCLUDE_DIRS})

# SOIL
#include(cmake/FindSOIL.cmake)
#if(NOT SOIL_FOUND)
#    message(FATAL_ERROR "SOIL not found!")
#    endif(NOT SOIL_FOUND)
#include_directories(${SOIL_INCLUDE_DIR})
#target_link_libraries(${PROJECT_NAME} ${SOIL_LIBRARY})

# Set Libraries
set(LIBRARIES glfw ${GLFW_LIBRARIES} GLEW ${GLEW_LIBRARIES} freeimage ${FreeImage_LIBRARIES} ${OPENGL_LIBRARY})
#set(LIBRARIES ${GLEW_LIBRARIES} ${SOIL_LIBRARY} ${GLFW_LIBRARIES} ${OPENGL_LIBRARY})
#set(LIBRARIES ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${OPENGL_LIBRARY})

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    #${CMAKE_SOURCE_DIR}/Models
    #${CMAKE_SOURCE_DIR}/Shader
    #${CMAKE_SOURCE_DIR}/Source
)

#file(GLOB Game_HEAD
#    Source/"*.cpp"
#)

#file(GLOB Game_SRC
#    Source/"*.cpp"
#)

set(Game_HEAD   src/main.h
    #src/inputCallback.h
                src/Camera.h
                src/Clouds.h
                src/Grass.h
                src/ImprovedPerlinNoise.h
                src/ModelLoader.h
                src/Noise.h
                src/Portal.h
                src/Shader.h
                src/Skydome.h
                src/Terrain.h
                src/Texture.h
                src/WorleyNoise.h
                src/PerlinNoise.h
		src/ImprovedPerlinNoise.h
		src/SimplexNoise.h
    )

set(Game_SRC    src/main.cpp
    #src/inputCallback.cpp
                src/Camera.cpp
                src/Clouds.cpp
                src/Grass.cpp
                src/ImprovedPerlinNoise.cpp
                src/ModelLoader.cpp
                src/Noise.cpp
                src/Portal.cpp
                src/Shader.cpp
                src/Skydome.cpp
                src/Terrain.cpp
                src/Texture.cpp
                src/WorleyNoise.cpp
                src/PerlinNoise.cpp
		src/ImprovedPerlinNoise.cpp
		src/SimplexNoise.cpp
    )

#message(${Game_SRC})
#message(${Game_HEAD})
add_executable(gates ${Game_SRC} ${Game_H})
#add_executable(game ${Game_SRC})
target_link_libraries(gates ${LIBRARIES} )


