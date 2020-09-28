cmake_minimum_required(VERSION 3.12...3.15)

if(EMSCRIPTEN)
    include("./emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
    set(CMAKE_STATIC_LIBRARY_SUFFIX ".bc")
    set(PLATFORM_CPP "PLATFORM_WEB")
    set(GRAPHICS "GRAPHICS_API_OPENGL_ES2")
    set(BGFX_CONFIG_RENDERER_WEBGPU True)
endif()

project(SDLBgfxTestProject
    VERSION 0.1
    DESCRIPTION "SDLBgfxTestProject"
    LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(SDL)
add_subdirectory(bgfx)
add_subdirectory(SDLBgfxTest)
