#include "Bgfx.hpp"

#include <cassert>
#include <cstdio>

#include <SDL_syswm.h>

#include <bgfx/platform.h>

bool Bgfx::Init(Window& window)
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

bool Bgfx::IsInitialized()
{
    return GetInstance().mInitialized;
}

bool Bgfx::Release()
{
    assert(GetInstance().mInitialized);
    GetInstance().mInitialized = false;
    bgfx::shutdown();
    return true;
}

Bgfx& Bgfx::GetInstance()
{
    static Bgfx instance;
    return instance;
}

Bgfx::Bgfx()
    : mInitialized(false)
{
}

Bgfx::~Bgfx()
{
    if (mInitialized)
    {
        printf("Bgfx hasn't been released\n");
        assert(false);
    }
}