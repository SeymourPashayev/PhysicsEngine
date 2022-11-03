cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
set(this_project engine)

project(${this_project} VERSION 0.0.1 LANGUAGES CXX)

set(headers
    include/engine.hpp
    include/math.hpp
    )

set (sources
    src/engine.cpp
    src/math.cpp
    )

find_package(SDL2 REQUIRED COMPONENTS SDL2)
add_executable(${this_project} ${headers} ${sources})
target_link_libraries(${this_project} PRIVATE SDL2::SDL2)

