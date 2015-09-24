cmake_minimum_required(VERSION 3.2)
project(MySceneGraph)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_EXE_LINKER_FLAGS "-static")

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
)

add_executable(MySceneGraph ${SOURCE_FILES})

target_link_libraries(MySceneGraph SDL2 glew32 opengl32 winmm imm32 Version)
