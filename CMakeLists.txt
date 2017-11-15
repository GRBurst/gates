cmake_minimum_required (VERSION 2.8)

project (GATES)

LIST(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

option(USE_CLANG "build application with clang" OFF) # OFF is the default
if(USE_CLANG)
    SET(CMAKE_CXX_COMPILER  "/usr/bin/clang++")
endif(USE_CLANG)

add_definitions(-std=c++14 -O2 -Wall -Wextra -pedantic)

# OpenGL
#find_package(OpenGL REQUIRED)
#if(NOT OPENGL_FOUND)
#    message(FATAL_ERROR "OpenGL not found!")
#endif(NOT OPENGL_FOUND)
#include_directories(${OPENGL_INCLUDE_DIR})
#target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})


# GLFW
#find_path(GLFW_INCLUDE GLFW/glfw.h)
#find_library(GLFW_LIBRARIES libglfw.a)
#include_directories(${GLFW_INCLUDE})

# GLEW
#find_path(GLEW_INCLUDE GL/glew.h ${EXTERNAL})
#find_library(GLEW_LIBRARIES libGLEW.a ${EXTERNAL})
#include_directories(${GLEW_INCLUDE})

# FreeImage
#find_path(FREEIMAGE_INCLUDE FreeImage.h ${EXTERNAL})
#find_library(FREEIMAGE_LIBRARY NAMES FreeImage freeimage libfreeimage libfreeimage-3.17.0.so PATHS ${EXTERNAL})
#include_directories(${FREEIMAGE_INCLUDE})

# Own stuff
FILE(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
FILE(GLOB_RECURSE HEADER_FILES "${CMAKE_HEADER_DIR}/src/*.h")
FILE(GLOB_RECURSE SHADER_FILES "${CMAKE_SOURCE_DIR}/src/shader/*.*")

# GLFW
FILE(GLOB_RECURSE HEADER_FILES_GLFW "${CMAKE_SOURCE_DIR}/external/glfw-3.1.2/include/*.h")
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/external/glfw-3.1.2/include)

SET(HEADER_FILES ${HEADER_FILES} ${HEADER_FILES_GLFW})
SET(GLFW_LIBRARIES ${CMAKE_SOURCE_DIR}/external/glfw-3.1.2/src/libglfw3.a)

# GLEW
FILE(GLOB_RECURSE HEADER_FILES_GLEW "${CMAKE_SOURCE_DIR}/extern/glew-1.13.0/include/*.h")
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/external/glew-1.13.0/include
                    ${CMAKE_SOURCE_DIR}/external/glew-1.13.0/deps)

SET(HEADER_FILES ${HEADER_FILES} ${HEADER_FILES_GLEW})
SET(GLEW_LIBRARIES ${CMAKE_SOURCE_DIR}/external/glew-1.13.0/lib/libGLEW.a)

# FreeImage
FILE(GLOB_RECURSE HEADER_FILES_FREEIMAGE "${CMAKE_SOURCE_DIR}/extern/FreeImage/Source/*.h")
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/external/FreeImage/Source)

SET(HEADER_FILES ${HEADER_FILES} ${HEADER_FILES_FREEIMAGE})
SET(FREEIMAGE_LIBRARIES ${CMAKE_SOURCE_DIR}/external/FreeImage/Dist/libfreeimage.a)

# Set additional libs
set( ADDITIONAL_LIBS 
     -pthread
     -lrt
     -lm
     -lXrandr
     -lXinerama
     -lX11
     -lGL
     -lXxf86vm
     -ldl
     -lXi
     -lXcursor
)

# Combine
set(LIBRARIES ${ADDITIONAL_LIBS} ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${FREEIMAGE_LIBRARIES} ${OPENGL_LIBRARY})

add_executable(gates ${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})
target_link_libraries(gates ${LIBRARIES} )

