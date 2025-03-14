cmake_minimum_required(VERSION 3.16)

project(week4)

find_package(SDL3 CONFIG REQUIRED)
find_package(flecs CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp
    Node.hpp
    Texture.hpp)

target_sources(${PROJECT_NAME} PRIVATE Texture.hpp Node.hpp Model.hpp Model.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 flecs::flecs)
