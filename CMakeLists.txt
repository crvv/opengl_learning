cmake_minimum_required(VERSION 3.2)
project(road)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 \
    -Wall -Wextra -Wpedantic \
    -Wconversion \
    -Wdouble-promotion \
    -Wshadow \
    -Wzero-as-null-pointer-constant \
    -Wuseless-cast")

set(SOURCE_FILES
        src/fps.h
        src/program.cpp
        src/program.h
        src/main.cpp
        src/monitor.cpp
        src/monitor.h
        src/renderer.cpp
        src/renderer.h
        src/controller.cpp
        src/controller.h
        src/node.cpp
        src/node.h
        src/glheader.h
        src/terrain.cpp
        src/terrain.h
        src/vao.cpp
        src/vao.h
        src/model.h
        src/texture.cpp
        src/texture.h
        src/spline.h
        src/json.hpp
        )

add_executable(road ${SOURCE_FILES})

if (WIN32)
    target_link_libraries(road SDL2 glew32 opengl32 winmm imm32 Version)
endif (WIN32)

if (APPLE)
    FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
endif(APPLE)

if (UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(SDL2 REQUIRED sdl2)
    pkg_search_module(GLEW REQUIRED glew)
    target_link_libraries(road
        ${SDL2_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${OpenGL_LIBRARY}
        )
    target_include_directories(road PUBLIC
        ${SDL2_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        )
endif(UNIX)

