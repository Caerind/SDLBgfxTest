#include <cassert>

#include <SDL.h>
#include <SDL_syswm.h>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include "SDL.hpp"
#include "Window.hpp"

class Bgfx
{
public:
	static constexpr bgfx::ViewId kClearView = 0;
    static constexpr unsigned int kClearColor = 0x443355FF;

	static bool Init(Window& window)
	{
        assert(!GetInstance().mInitialized);

		SDL_SysWMinfo wmi;
		SDL_VERSION(&wmi.version);
		if (!SDL_GetWindowWMInfo(window.mWindow, &wmi))
		{
			return false;
		}

		bgfx::PlatformData pd;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
		pd.ndt = wmi.info.x11.display;
		pd.nwh = (void*)(uintptr_t)wmi.info.x11.window;
#elif BX_PLATFORM_OSX
		pd.ndt = NULL;
		pd.nwh = wmi.info.cocoa.window;
#elif BX_PLATFORM_WINDOWS
		pd.ndt = NULL;
		pd.nwh = wmi.info.win.window;
#elif BX_PLATFORM_STEAMLINK
		pd.ndt = wmi.info.vivante.display;
		pd.nwh = wmi.info.vivante.window;
#endif // BX_PLATFORM_
		pd.context = NULL;
		pd.backBuffer = NULL;
		pd.backBufferDS = NULL;
		bgfx::setPlatformData(pd);

        bgfx::renderFrame();

		bgfx::Init init;
		init.type = bgfx::RendererType::Count; // Automatically choose a renderer
		init.resolution.width = window.GetWidth();
		init.resolution.height = window.GetHeight();
		init.resolution.reset = BGFX_RESET_VSYNC;
		bgfx::init(init);

		bgfx::setDebug(BGFX_DEBUG_TEXT | BGFX_DEBUG_STATS);

		bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, kClearColor, 1.0f, 0);
		bgfx::setViewRect(kClearView, 0, 0, static_cast<uint16_t>(window.GetWidth()), static_cast<uint16_t>(window.GetHeight()));

        GetInstance().mInitialized = true;
        return true;
	}

    static bool IsInitialized()
    {
        return GetInstance().mInitialized;
    }

    static bool Release()
    {
		assert(GetInstance().mInitialized);
		GetInstance().mInitialized = false;
        bgfx::shutdown();
		return true;
    }

private:
    Bgfx()
        : mInitialized(false)
    {
    }

    ~Bgfx()
    {
        assert(!mInitialized);
    }

    static Bgfx& GetInstance()
    {
        static Bgfx instance;
        return instance;
    }

    bool mInitialized;
};

int main(int argc, char** argv)
{
    SDL::Init();
    {
        Window window;
        if (!window.Create("SDL & bgfx", 800, 600))
        {
            return -1;
        }

        Bgfx::Init(window);

        int counter = 0;
        while (window.IsOpen())
        {
            SDL_Event event;
            while (SDL::PollEvent(event))
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

            bgfx::touch(Bgfx::kClearView);
			bgfx::dbgTextClear();
			bgfx::frame();
        }

        Bgfx::Release();
    }
    SDL::Release();
	return 0;
}