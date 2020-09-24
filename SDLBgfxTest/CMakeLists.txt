add_executable(SDLBGFXTest SDL.hpp Window.hpp main.cpp)
target_link_libraries(SDLBGFXTest PUBLIC SDL2 SDL2main bgfx)
