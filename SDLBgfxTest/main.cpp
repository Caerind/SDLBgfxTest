#include "EngineIntegration.hpp"

#include <vector>

#include <bx/math.h>

#include "SDLWrapper.hpp"
#include "Window.hpp"
#include "BgfxWrapper.hpp"
#include "ImGuiWrapper.hpp"
#include "Shader.hpp"
#include "Sprite.hpp"
#include "Texture.hpp"
#include "Mouse.hpp"
#include "Keyboard.hpp"

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
            Texture texture;
            if (!texture.Initialize("fieldstone-rgba.dds"))
			{
				window.Close();
#ifdef ENGINE_IMGUI
                ImGuiWrapper::Release();
#endif // ENGINE_IMGUI
				BgfxWrapper::Release();
				window.Destroy();
				SDLWrapper::Release();
				Debug("Can't load texture\n");
                return -1;
            }

            std::vector<Sprite> sprites;
            sprites.resize(1000);
            for (auto& sprite : sprites)
			{
				sprite.SetTexture(texture);
            }

            const bx::Vec3 up = { 0.0f, 1.0f, 0.0f };
            const bx::Vec3 at = { 0.0f, 0.0f, 0.0f };
            const bx::Vec3 eye = { 0.0f, 0.0f, 5.0f };
            F32 view[16];
            bx::mtxLookAt(view, eye, at, up, bx::Handness::Right);
            F32 proj[16];
            bx::mtxProj(proj, 60.0f, F32(window.GetWidth()) / F32(window.GetHeight()), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth, bx::Handness::Right);

            U32 counter = 0;

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

				bgfx::setViewTransform(BgfxWrapper::kClearView, view, proj);

				for (auto& sprite : sprites)
				{
					sprite.Render();
				}

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