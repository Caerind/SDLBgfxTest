add_executable(SDLBGFXTest
	shaders/sprite.vs
	shaders/sprite.fs
	shaders/varying.def.sc
	EngineIntegration.hpp 
	SDLWrapper.hpp 
	SDLWrapper.cpp 
	Window.hpp 
	Window.cpp 
	BgfxWrapper.hpp 
	BgfxWrapper.cpp 
	Shader.hpp 
	Shader.cpp
	Texture.hpp
	Texture.cpp
	Sprite.hpp 
	Sprite.cpp 
	ImGuiWrapper.hpp 
	ImGuiWrapper.cpp
	Mouse.hpp
	Mouse.cpp
	Keyboard.hpp
	Keyboard.cpp
	Camera.hpp
	Camera.cpp
	main.cpp)

if(EMSCRIPTEN)
    include("../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
	#set_target_properties(client SDLBGFXTest LINK_FLAGS "-s DEMANGLE_SUPPORT=1 --preload-file assets --bind")
	set_target_properties(SDLBGFXTest PROPERTIES LINK_FLAGS "-Os -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"png\"]' -s USE_GLFW=3 -s TOTAL_MEMORY=16777216 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=1 --preload-file resources -s ASSERTIONS=1 --profiling --shell-file $(RAYLIB_PATH)/src/shell.html")
	#target_link_libraries(*.bc)
	link_directories("../emsdk/upstream/emscripten/cache/wasm/")
	target_link_libraries(SDLBGFXTest PUBLIC gl)
	message(AUTHOR_WARNING "Debug: Target properties est to match emscripten requirements")
endif()

target_link_libraries(SDLBGFXTest PUBLIC SDL2 SDL2main bgfx dear-imgui)
