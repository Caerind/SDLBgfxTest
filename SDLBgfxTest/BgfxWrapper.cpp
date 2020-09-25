#include "BgfxWrapper.hpp"

#include <SDL.h>
#include <SDL_syswm.h>

#include <bgfx/platform.h>

#include "Sprite.hpp"

namespace NAMESPACE_NAME
{

bool BgfxWrapper::Init(Window& window)
{
    BgfxWrapper& bgfx = GetInstance();

    assert(!bgfx.mInitialized);

    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(window.mWindow, &wmi))
    {
        return false;
    }

    bgfx::PlatformData pd;

    switch (wmi.subsystem)
    {
#ifdef ENGINE_PLATFORM_WINDOWS
    case SDL_SYSWM_WINDOWS:
	{
		pd.ndt = nullptr;
		pd.nwh = wmi.info.win.window;
        break;
    }
#endif // ENGINE_PLATFORM_WINDOWS

#ifdef ENGINE_PLATFORM_WINRT
    case SDL_SYSWM_WINRT:
    {
		pd.ndt = nullptr;
		pd.nwh = wmi.info.winrt.window;
        break;
    }
#endif // ENGINE_PLATFORM_WINRT

#ifdef ENGINE_PLATFORM_X11
    case SDL_SYSWM_X11:
    {
        pd.ndt = wmi.info.x11.display;
        pd.nwh = (void*)(uintptr_t)wmi.info.x11.window;
        break;
    }
#endif // ENGINE_PLATFORM_X11

#ifdef ENGINE_PLATFORM_DIRECTFB
	case SDL_SYSWM_DIRECTFB:
	{
		pd.ndt = nullptr;
		pd.nwh = wmi.info.dfb.window;
		break;
	}
#endif // ENGINE_PLATFORM_DIRECTFB

#ifdef ENGINE_PLATFORM_COCOA // OSX
	case SDL_SYSWM_COCOA:
	{
		pd.ndt = nullptr;
		pd.nwh = wmi.info.cocoa.window;
		break;
	}
#endif // ENGINE_PLATFORM_COCOA

#ifdef ENGINE_PLATFORM_UIKIT // iOS
	case SDL_SYSWM_UIKIT:
	{
		pd.ndt = nullptr;
		pd.nwh = wmi.info.uikit.window;
		break;
	}
#endif // ENGINE_PLATFORM_UIKIT

#ifdef ENGINE_PLATFORM_WAYLAND
	case SDL_SYSWM_WAYLAND:
	{
		pd.ndt = nullptr;
		pd.nwh = (void*)(uintptr_t)wmi.info.wl.display;
		break;
	}
#endif // ENGINE_PLATFORM_WAYLAND

#ifdef ENGINE_PLATFORM_MIR
	case SDL_SYSWM_MIR:
	{
		pd.ndt = nullptr;
		pd.nwh = nullptr;
		break;
	}
#endif // ENGINE_PLATFORM_MIR

#ifdef ENGINE_PLATFORM_ANDROID
	case SDL_SYSWM_ANDROID:
	{
		pd.ndt = nullptr;
		pd.nwh = wmi.info.android.window;
		break;
	}
#endif // ENGINE_PLATFORM_ANDROID

#ifdef ENGINE_PLATFORM_VIVANTE // Steamlink
    case SDL_SYSWM_VIVANTE:
	{
        pd.ndt = wmi.info.vivante.display;
		pd.nwh = (void*)(uintptr_t)wmi.info.vivante.window;
		break;
	}
#endif // ENGINE_PLATFORM_VIVANTE

    case SDL_SYSWM_UNKNOWN:
    default:
    {
		Debug("SDL cannot detect the platform\n");
        return false;
    }
    }

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    pd.ndt = wmi.info.x11.display;
    pd.nwh = (void*)(uintptr_t)wmi.info.x11.window;
#elif BX_PLATFORM_OSX
    pd.ndt = NULL;
    pd.nwh = wmi.info.cocoa.window;
#elif BX_PLATFORM_WINDOWS
#elif BX_PLATFORM_STEAMLINK
    pd.ndt = wmi.info.vivante.display;
    pd.nwh = wmi.info.vivante.window;
#endif // BX_PLATFORM_
    pd.context = NULL;
    pd.backBuffer = NULL;
    pd.backBufferDS = NULL;
    bgfx::setPlatformData(pd);

    bgfx::Init init;
    init.type = bgfx::RendererType::Count; // Automatically choose a renderer
    init.resolution.width = window.GetWidth();
    init.resolution.height = window.GetHeight();
    init.resolution.reset = BGFX_RESET_VSYNC;
    bgfx::init(init);

#ifdef ENGINE_DEBUG
    bgfx::setDebug(BGFX_DEBUG_TEXT | BGFX_DEBUG_STATS);
#endif // ENGINE_DEBUG

    bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, kClearColor, 1.0f, 0);
    bgfx::setViewRect(kClearView, 0, 0, static_cast<uint16_t>(window.GetWidth()), static_cast<uint16_t>(window.GetHeight()));

    if (!Sprite::InitializeSprites())
    {
        return false;
    }

    bgfx.mInitialized = true;
    return true;
}

bool BgfxWrapper::IsInitialized()
{
    return GetInstance().mInitialized;
}

bool BgfxWrapper::Release()
{
    BgfxWrapper& bgfx = GetInstance();

	assert(bgfx.mInitialized);

    Sprite::ReleaseSprites();

	bgfx::shutdown();
    bgfx.mInitialized = false;

    return true;
}

BgfxWrapper& BgfxWrapper::GetInstance()
{
    static BgfxWrapper instance;
    return instance;
}

BgfxWrapper::BgfxWrapper()
    : mInitialized(false)
{
}

BgfxWrapper::~BgfxWrapper()
{
    assert(!mInitialized);
}

} // namespace NAMESPACE_NAME