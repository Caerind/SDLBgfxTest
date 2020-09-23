#include <cassert>

#include <SDL.h>
#include <SDL_syswm.h>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

class SDL
{
public:
    static bool Init()
	{
		assert(!GetInstance().mInitialized);
        GetInstance().mInitialized = SDL_Init(SDL_INIT_EVERYTHING) >= 0;
        return IsInitialized();
    }

    static bool IsInitialized()
    {
        return GetInstance().mInitialized;
    }

    static bool Release()
    {
        assert(GetInstance().mInitialized);
        GetInstance().mInitialized = false;
        SDL_Quit();
        return true;
    }

    static const char* GetError()
    {
        return SDL_GetError();
    }

private:
    SDL() 
        : mInitialized(false) 
    {
    }

    ~SDL()
    {
        assert(!mInitialized);
    }

	static SDL& GetInstance()
	{
		static SDL instance;
		return instance;
	}

    bool mInitialized;
};

class Surface
{
public:
    Surface()
        : mSurface(nullptr)
        , mAllocated(false)
    {
    }

    ~Surface()
    {
        Destroy();
    }

    bool CreateFromBMP(const char* filename)
    {
        mSurface = SDL_LoadBMP(filename);
        if (mSurface == nullptr)
        {
            return false;
        }
        else
        {
			mAllocated = true;
			return true;
        }
    }

    void Destroy()
    {
        if (mAllocated)
        {
            if (mSurface != nullptr)
            {
                SDL_FreeSurface(mSurface);
            }
            mAllocated = false;
        }
        mSurface = nullptr;
    }

    void Fill(int r, int g, int b)
	{
		if (mSurface != nullptr)
		{
			SDL_FillRect(mSurface, nullptr, SDL_MapRGB(mSurface->format, r, g, b));
		}
    }

    void CopyTo(Surface& surface)
    {
        if (mSurface != nullptr && surface.mSurface != nullptr)
        {
            SDL_BlitSurface(mSurface, nullptr, surface.mSurface, nullptr);
        }
    }

private:
    friend class Window;
    void InitializeWindowSurface(SDL_Surface* surface)
    {
        mSurface = surface;
        mAllocated = false;
        Fill(255, 255, 255);
    }

    SDL_Surface* mSurface;
    bool mAllocated;
};

class Window
{
public:
    Window()
        : mWindow(nullptr)
        , mWidth(0)
        , mHeight(0)
    {
    }

    ~Window()
    {
        Close();
    }

    bool Create(const char* name, int width, int height)
    {
		mWindow = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (IsOpen())
		{
            mWidth = width;
            mHeight = height;
        }
        return IsOpen();
    }

    void Display()
    {
        if (IsOpen())
        {
            SDL_UpdateWindowSurface(mWindow);
        }
    }

    void Close()
    {
        if (IsOpen())
		{
			SDL_DestroyWindow(mWindow);
            mWindow = nullptr;
        }
    }

    bool IsOpen() const
    {
        return mWindow != nullptr;
    }

    bool PollEvent(SDL_Event& event)
    {
        if (IsOpen())
        {
            return SDL_PollEvent(&event);
        }
        else
        {
            return false;
        }
	}

    int GetWidth() const { return mWidth; }
    int GetHeight() const { return mHeight; }

private:
    friend class Bgfx;

    SDL_Window* mWindow;
    int mWidth;
    int mHeight;
};

class Bgfx
{
public:
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
		//bgfx::reset(window.GetWidth(), window.GetHeight(), BGFX_RESET_VSYNC); // TODO: Is it needed ?

		bgfx::setDebug(BGFX_DEBUG_TEXT | BGFX_DEBUG_STATS);

		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF /*purple*/, 1.f, 0);
		bgfx::setViewRect(0, 0, 0, static_cast<uint16_t>(window.GetWidth()), static_cast<uint16_t>(window.GetHeight()));
		// bgfx::frame(); // TODO: Is it needed/better ?

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
        if (!window.Create("SDL", 800, 600))
        {
            return -1;
        }

        Bgfx::Init(window);

        int counter = 0;
        while (window.IsOpen())
        {
            SDL_Event event;
            while (window.PollEvent(event))
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    window.Close();
                    break;
                case SDL_WINDOWEVENT: {
                    const SDL_WindowEvent& wev = event.window;
                    switch (wev.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        break;

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

			bgfx::dbgTextClear();
			bgfx::frame();
        }

        Bgfx::Release();
    }
    SDL::Release();
	return 0;
}