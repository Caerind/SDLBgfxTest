add_executable(SDLBGFXTest SDL.hpp SDL.cpp Window.hpp Window.cpp Bgfx.hpp Bgfx.cpp main.cpp)
target_link_libraries(SDLBGFXTest PUBLIC SDL2 SDL2main bgfx)
