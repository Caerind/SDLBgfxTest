#include "EngineIntegration.hpp"

#include "SDLWrapper.hpp"
#include "Window.hpp"
#include "BgfxWrapper.hpp"
#include "ImGuiWrapper.hpp"

using namespace NAMESPACE_NAME;

struct PosColorVertex
{
    F32 x;
    F32 y;
    F32 z;
    U32 abgr;

    static const bgfx::VertexLayout kLayout; 
};
const bgfx::VertexLayout PosColorVertex::kLayout = []()
{
	bgfx::VertexLayout layout;
	layout.begin();
	layout.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float);
	layout.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true);
	layout.end();
	return layout;
}();

int main(int argc, char** argv)
{
    SDLWrapper::Init();
    {
        Window window;
        if (!window.Create("SDL & bgfx", 800, 600))
        {
            return -1;
        }

		BgfxWrapper::Init(window);
#ifdef ENGINE_IMGUI
        ImGuiWrapper::Init();
#endif // ENGINE_IMGUI

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
			bgfx::dbgTextClear();
			bgfx::frame();
        }

#ifdef ENGINE_IMGUI
        ImGuiWrapper::Release();
#endif // ENGINE_IMGUI
        BgfxWrapper::Release();
    }
    SDLWrapper::Release();
    return 0;
}