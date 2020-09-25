#include "EngineIntegration.hpp"

#include <bx/math.h>

#include "SDLWrapper.hpp"
#include "Window.hpp"
#include "BgfxWrapper.hpp"
#include "ImGuiWrapper.hpp"
#include "Shader.hpp"
#include "Sprite.hpp"

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
			SDLWrapper::Release();
			Debug("Can't initialize Bgfx\n");
			return -1;
        }
#ifdef ENGINE_IMGUI
        if (!ImGuiWrapper::Init())
		{
            BgfxWrapper::Release();
			SDLWrapper::Release();
            Debug("Can't initialize ImGui\n");
            return -1;
        }
#endif // ENGINE_IMGUI

        {
            Sprite sprite;
            sprite.SetTextureRect(0, 0, 1, 1); // Just to create the VB

            const bx::Vec3 at = { 0.0f, 0.0f,   0.0f };
            const bx::Vec3 eye = { 0.0f, 0.0f, 10.0f };
            F32 view[16];
            bx::mtxLookAt(view, eye, at);
            F32 proj[16];
            bx::mtxProj(proj, 60.0f, F32(window.GetWidth()) / F32(window.GetHeight()), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);

            bgfx::setViewTransform(0, view, proj);

            while (window.IsOpen())
            {
                SDL_Event event;
                while (SDLWrapper::PollEvent(event))
                {
                    switch (event.type)
                    {
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
                            window.Close();
                            break;
                        }
                    } break;
                    }
                }
                if (!window.IsOpen())
                {
                    break;
                }

                bgfx::touch(BgfxWrapper::kClearView);
                //bgfx::dbgTextClear();

                sprite.Render();

                bgfx::frame();
            }
        }

#ifdef ENGINE_IMGUI
        ImGuiWrapper::Release();
#endif // ENGINE_IMGUI
        BgfxWrapper::Release();
    }
    SDLWrapper::Release();
    return 0;
}