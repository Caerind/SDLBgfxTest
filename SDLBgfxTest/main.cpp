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

int main(int argc, char** argv)
{
    if (!SDLWrapper::Init())
	{
		Debug("Can't initialize SDL\n");
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
                //camera.InitializeOrthographic(-10.0f, -10.0f, 10.0f, 10.0f, 0.1f, 100.0f);

                // View
                camera.SetPosition(Vector3f(0.0f, 0.0f, 5.0f));
                camera.SetLookAt(Vector3f(0.0f, 0.0f, 0.0f));
                camera.SetUpVector(Vector3f(0.0f, 1.0f, 0.0f));
            }

            const F32 cameraSpeedZ = 2.0f;
            const F32 cameraSpeedX = 3.0f;

			I64 last = bx::getHPCounter();
			const double freq = double(bx::getHPFrequency());

			const bgfx::ViewId imguiViewId = 250;
			bool imguiDemoVisible = true;

            while (!window.ShouldClose())
            {
                Mouse::Refresh();
                Keyboard::Refresh();

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
                if (window.ShouldClose())
                {
                    bgfx::frame();
                    break;
                }

#ifdef ENGINE_IMGUI
                ImGuiWrapper::BeginFrame(imguiViewId);
                ImGui::ShowDemoWindow(&imguiDemoVisible);
                ImGuiWrapper::EndFrame();
#endif // ENGINE_DEBUG

				const I64 now = bx::getHPCounter();
				const I64 frameTime = now - last;
				last = now;
				const F32 dt = F32(frameTime / freq);

                bool moved = false;
				Vector3f pos = camera.GetPosition();
                const F32 mvtZ = dt * cameraSpeedZ;
                if (Keyboard::IsHold(Keyboard::Key::Up))
                {
					pos.z -= mvtZ;
                    moved = true;
                }
                if (Keyboard::IsHold(Keyboard::Key::Down))
				{
					pos.z += mvtZ;
					moved = true;
				}
				const F32 mvtX = dt * cameraSpeedX;
				if (Keyboard::IsHold(Keyboard::Key::Right))
				{
                    pos.x += mvtX;
					moved = true;
				}
                if (Keyboard::IsHold(Keyboard::Key::Left))
                {
					pos.x -= mvtX;
					moved = true;
                }
                if (moved)
                {
					camera.SetPosition(pos);
                }

                if (Keyboard::IsControlHold() && Keyboard::IsPressed(Keyboard::Key::F3))
				{
#ifdef ENGINE_DEBUG
					BgfxWrapper::ToggleDisplayStats();
#endif // ENGINE_DEBUG
                }

                bgfx::touch(BgfxWrapper::kClearView);

                bgfx::dbgTextClear();
                const Vector2i dbgMousePos = Mouse::GetPositionCurrentWindow();
                const Vector2i dbgMouseDeltaPos = Mouse::GetDeltaPosition();
                const I32 dbgMouseWheel = Mouse::GetWheel();
                bgfx::dbgTextPrintf(0, 0, 0x0f, "Mouse: (%d, %d) (%d, %d) (%d)", dbgMousePos.x, dbgMousePos.y, dbgMouseDeltaPos.x, dbgMouseDeltaPos.y, dbgMouseWheel);

                camera.Apply(BgfxWrapper::kClearView);

                spriteA.Render();

                bgfx::setTransform(spriteBTransform);
                spriteB.Render();

                bgfx::frame();
            }
        }

#ifdef ENGINE_IMGUI
        ImGuiWrapper::Release();
#endif // ENGINE_IMGUI
		BgfxWrapper::Release();
	}
	SDLWrapper::Release();
    Debug("Exited properly");
    return 0;
}