add_executable(SDLBGFXTest EngineIntegration.hpp SDLWrapper.hpp SDLWrapper.cpp Window.hpp Window.cpp BgfxWrapper.hpp BgfxWrapper.cpp Shader.hpp Shader.cpp ImGuiWrapper.hpp ImGuiWrapper.cpp main.cpp)
target_link_libraries(SDLBGFXTest PUBLIC SDL2 SDL2main bgfx)
