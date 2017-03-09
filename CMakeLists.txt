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

target_link_libraries(road SDL2 glew32 opengl32 winmm imm32 Version)
