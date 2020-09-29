#include "EngineIntegration.hpp"

#include <bx/math.h>
#include <bx/timer.h>

#include "SDLWrapper.hpp"
#include "Window.hpp"
#include "BgfxWrapper.hpp"
#include "ImGuiWrapper.hpp"
#include "Shader.hpp"
#include "Sprite.hpp"
#include "Texture.hpp"
#include "Mouse.hpp"
#include "Keyboard.hpp"
#include "Camera.hpp"

using namespace NAMESPACE_NAME;

bool app(Window& window);

void CameraUpdate(Camera& camera, F32 deltaTime);

int main(int argc, char** argv)
{
    if (!SDLWrapper::Init())
	{
		Debug("Can't initialize SDL : %s\n", SDLWrapper::GetError());
        return -1;
    }
    {
        Window window;
        if (!window.Create("SDL & bgfx", 800, 600))
        {
            SDLWrapper::Release();
            Debug("Can't create Window\n");
            return -1;
        }

        if (!BgfxWrapper::Init(window))
		{
            window.Close();
            window.Destroy();
			SDLWrapper::Release();
			Debug("Can't initialize Bgfx\n");
			return -1;
        }
#ifdef ENGINE_IMGUI
        if (!ImGuiWrapper::Init())
		{
            window.Close();
			BgfxWrapper::Release();
			window.Destroy();
			SDLWrapper::Release();
            Debug("Can't initialize ImGui\n");
            return -1;
        }
#endif // ENGINE_IMGUI

		// This is the main function without all the init SDL/Window/Bgfx/ImGui
		app(window);

#ifdef ENGINE_IMGUI
        ImGuiWrapper::Release();
#endif // ENGINE_IMGUI
		BgfxWrapper::Release();
	}
	SDLWrapper::Release();
    Debug("Exited properly");
    return 0;
}

bool app(Window& window)
{
	Texture textureA;
	const char* textureAFilename = "fieldstone-rgba.dds";
	if (!textureA.Initialize(textureAFilename))
	{
		Debug("Can't load texture A : %s\n", textureAFilename);
	}

	Texture textureB;
	const char* textureBFilename = "ship_default.png";
	if (!textureB.Initialize(textureBFilename))
	{
		Debug("Can't load texture B : %s\n", textureBFilename);
	}

	Sprite spriteA;
	spriteA.SetTexture(textureA);

	Sprite spriteB;
	spriteB.SetTexture(textureB);
	F32 spriteBTransform[16];
	bx::mtxTranslate(spriteBTransform, 2.0f, 2.0f, 0.0f);

	Camera camera;
	{
		// Projection
		camera.InitializePerspective(60.0f, F32(window.GetWidth()) / F32(window.GetHeight()), 0.1f, 100.0f);

		// View
		camera.SetPosition(Vector3f(0.0f, 0.0f, 5.0f));
		camera.SetLookAt(Vector3f(0.0f, 0.0f, 0.0f));
		camera.SetUpVector(Vector3f(0.0f, 1.0f, 0.0f));
	}

	I64 lastTime = bx::getHPCounter();
	const double frequency = double(bx::getHPFrequency());

	const bgfx::ViewId imguiViewId = 250;

	while (!window.ShouldClose())
	{
		// Begin frame
		Mouse::Refresh();
		Keyboard::Refresh();

		// Event loop
		SDL_Event event;
		while (SDLWrapper::PollEvent(event))
		{
			switch (event.type)
			{
			case SDL_MOUSEWHEEL:
			case SDL_MOUSEBUTTONDOWN:
			{
				Mouse::HandleEvent(event);
				break;
			}
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			case SDL_TEXTINPUT:
			{
				Keyboard::HandleEvent(event);
				break;
			}
			case SDL_QUIT:
			{
				window.Close();
				break;
			}
			case SDL_WINDOWEVENT:
			{
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_CLOSE:
				{
					window.Close();
					break;
				}
				}
				break;
			}
			}
		}

		// ImGui
#ifdef ENGINE_IMGUI
		ImGuiWrapper::BeginFrame(imguiViewId);
		ImGui::ShowDemoWindow();
		ImGuiWrapper::EndFrame();
#endif // ENGINE_DEBUG

		// Compute dt
		const I64 now = bx::getHPCounter();
		const I64 frameTime = now - lastTime;
		lastTime = now;
		const F32 dt = F32(frameTime / frequency);

		F32 rot[16];
		bx::mtxRotateY(rot, 3.14152f * dt);
		F32 copy[16];
		for (U32 i = 0; i < 16; ++i)
			copy[i] = spriteBTransform[i];
		bx::mtxMul(spriteBTransform, copy, rot);

		CameraUpdate(camera, dt);

		// Toggle debug stats
#ifdef ENGINE_DEBUG
		if (Keyboard::IsControlHold() && Keyboard::IsPressed(Keyboard::Key::F3))
		{
			BgfxWrapper::ToggleDisplayStats();
		}
#endif // ENGINE_DEBUG

		// Render
		{
			bgfx::touch(BgfxWrapper::kClearView);

			// Display mouse pos
			bgfx::dbgTextClear();
			const Vector2i dbgMousePos = Mouse::GetPositionCurrentWindow();
			const Vector2i dbgMouseDeltaPos = Mouse::GetDeltaPosition();
			const I32 dbgMouseWheel = Mouse::GetWheel();
			bgfx::dbgTextPrintf(0, 0, 0x0f, "Mouse: (%d, %d) (%d, %d) (%d)", dbgMousePos.x, dbgMousePos.y, dbgMouseDeltaPos.x, dbgMouseDeltaPos.y, dbgMouseWheel);

			camera.Apply(BgfxWrapper::kClearView);

			spriteA.Render(BgfxWrapper::kClearView);

			bgfx::setTransform(spriteBTransform);
			spriteB.Render(BgfxWrapper::kClearView);

			bgfx::frame();
		}
	}

    return true;
}

// Inspired by TotalWar controls
void CameraUpdate(Camera& camera, F32 deltaTime)
{
	Vector3f movement;
	bool moved = false;

	const F32 cameraSpeedYWheel = 1.0f;
	if (Mouse::GetWheel() != 0)
	{
		movement.y += cameraSpeedYWheel * Mouse::GetWheel();
		moved = true;
	}

	const F32 cameraSpeedZ = 2.0f;
	const F32 mvtZ = deltaTime * cameraSpeedZ;
	if (Keyboard::IsHold(Keyboard::Key::Up))
	{
		movement.z -= mvtZ;
		moved = true;
	}
	if (Keyboard::IsHold(Keyboard::Key::Down))
	{
		movement.z += mvtZ;
		moved = true;
	}

	const F32 cameraSpeedX = 3.0f;
	const F32 mvtX = deltaTime * cameraSpeedX;
	if (Keyboard::IsHold(Keyboard::Key::Right))
	{
		movement.x += mvtX;
		moved = true;
	}
	if (Keyboard::IsHold(Keyboard::Key::Left))
	{
		movement.x -= mvtX;
		moved = true;
	}
	
	if (moved)
	{
		camera.Move(movement);
	}
}