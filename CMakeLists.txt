cmake_minimum_required(VERSION 3.12...3.15)

project(SDLBgfxTestProject
    VERSION 0.2
    DESCRIPTION "SDLBgfxTestProject"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(SDL)
add_subdirectory(bgfx)
add_subdirectory(SDLBgfxTest)
