set(SRC_MATH
	Math/AABB.hpp
	Math/Color.hpp
	Math/Frustum.hpp
	Math/Math.hpp
	Math/Matrix3.hpp
	Math/Matrix4.hpp
	Math/Plane.hpp
	Math/Quaternion.hpp
	Math/Ray.hpp
	Math/Rect.hpp
	Math/Sphere.hpp
	Math/Vector2.hpp
	Math/Vector3.hpp
	Math/Vector4.hpp
)
source_group("Math" FILES ${SRC_MATH})

set(SRC_WINDOW
	Window/SDLWrapper.hpp 
	Window/SDLWrapper.cpp 
	Window/Window.hpp 
	Window/Window.cpp
	Window/Mouse.hpp
	Window/Mouse.cpp
	Window/Keyboard.hpp
	Window/Keyboard.cpp
	Window/Controller.hpp
	Window/Controller.cpp
	Window/EventSystem.hpp
	Window/EventSystem.cpp
)
source_group("Window" FILES ${SRC_WINDOW})

set(SRC_GRAPHICS
	Graphics/BgfxWrapper.hpp 
	Graphics/BgfxWrapper.cpp 
	Graphics/Shader.hpp 
	Graphics/Shader.cpp
	Graphics/Texture.hpp
	Graphics/Texture.cpp
	Graphics/Sprite.hpp 
	Graphics/Sprite.cpp 
	Graphics/ImGuiWrapper.hpp 
	Graphics/ImGuiWrapper.cpp
	Graphics/Mesh.hpp
	Graphics/Mesh.cpp
	Graphics/Camera.hpp
	Graphics/Camera.cpp
)
source_group("Graphics" FILES ${SRC_GRAPHICS})

set(SRC_SHADERS
	shaders/sprite.vs
	shaders/sprite.fs
	shaders/varying.def.sc
)
source_group("Shaders" FILES ${SRC_SHADERS})

set(SRC_GAME
	Game/Cameras.hpp
)
source_group("Game" FILES ${SRC_GAME})

set(SRC_GLOBAL
	EngineIntegration.hpp
	main.cpp
)
source_group("" FILES ${SRC_GLOBAL})
	
add_executable(SDLBGFXTest
	${SRC_MATH}
	${SRC_WINDOW}
	${SRC_GRAPHICS}
	${SRC_GAME}
	${SRC_SHADERS}
	${SRC_GLOBAL})
target_link_libraries(SDLBGFXTest PUBLIC SDL2 SDL2main bgfx dear-imgui)
