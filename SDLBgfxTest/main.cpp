#include <SDL.h>
#include <cassert>

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
        , mSurface()
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
			mSurface.InitializeWindowSurface(SDL_GetWindowSurface(mWindow));
			SDL_UpdateWindowSurface(mWindow);
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

    Surface& GetSurface() { return mSurface; }
    const Surface& GetSurface() const { return mSurface; }

private:
    SDL_Window* mWindow;
    Surface mSurface;
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

        Surface image;
        if (!image.CreateFromBMP("test.bmp"))
        {
            return -1;
        }

        image.CopyTo(window.GetSurface());

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

			SDL_Delay(16);

			window.Display();
        }
    }
    SDL::Release();
	return 0;
}