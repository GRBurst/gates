cmake_minimum_required (VERSION 2.8)

project (GATES)

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
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
if(NOT GLFW_FOUND)
    message(FATAL_ERROR "GLFW not found")
endif(NOT GLFW_FOUND)
include_directories(${GLFW_INCLUDE_DIRS})
#target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARIES})

# GLEW
find_package(GLEW REQUIRED)
if(NOT GLEW_FOUND)
    message(FATAL_ERROR "GLEW not found!")
endif(NOT GLEW_FOUND)
include_directories(${GLEW_INCLUDE_DIRS})
#target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})

# SOIL
#include(FindSOIL.cmake)
#if(NOT SOIL_FOUND)
    #message(FATAL_ERROR "SOIL not found!")
    #endif(NOT SOIL_FOUND)
#include_directories(${SOIL_INCLUDE_DIR})
#target_link_libraries(${PROJECT_NAME} ${SOIL_LIBRARY})

# Set Libraries
#set(LIBRARIES ${GLEW_LIBRARIES} ${SOIL_LIBRARY} ${GLFW_LIBRARIES} ${OPENGL_LIBRARY})
set(LIBRARIES ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${OPENGL_LIBRARY})

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
                src/camera.h
                src/keyCallback.h
                src/Shader.h
    )

set(Game_SRC    src/main.cpp
                src/camera.cpp
                src/keyCallback.cpp
                src/Shader.cpp
    )

#message(${Game_SRC})
#message(${Game_HEAD})
add_executable(gates ${Game_SRC} ${Game_H})
#add_executable(game ${Game_SRC})
target_link_libraries(gates ${LIBRARIES} )

