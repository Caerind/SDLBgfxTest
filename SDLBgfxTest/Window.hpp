#include <cassert>

#include <SDL.h>

// TODO : https://alexandre-laurent.developpez.com/tutoriels/sdl-2/creer-premieres-fenetres

class Window
{
public:
    Window()
        : mWindow(nullptr)
    {
    }

    Window(const char* name, int width, int height)
        : Window()
    {
        Create(name, width, height);
    }

    ~Window()
    {
        Close();
    }

    bool Create(const char* name, int width, int height, unsigned int flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)
    {
		mWindow = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
        return mWindow != nullptr;
    }

    void Close()
    {
        if (mWindow != nullptr)
		{
			SDL_DestroyWindow(mWindow);
            mWindow = nullptr;
        }
    }

    bool IsOpen() const
    {
        return mWindow != nullptr;
    }

    void SetVisible(bool visible)
    {
        if (mWindow != nullptr)
		{
			if (!visible)
			{
                SDL_HideWindow(mWindow);
			}
			else
			{
                SDL_ShowWindow(mWindow);
			}
        }
    }
    bool IsVisible() const
    {
        return (mWindow != nullptr) ? (SDL_GetWindowFlags(mWindow) & SDL_WINDOW_SHOWN) > 0 : false;
    }

    void Minimize()
    {
        if (mWindow != nullptr)
        {
            SDL_MinimizeWindow(mWindow);
        }
    }
    void Maximize()
    {
        if (mWindow != nullptr)
        {
            SDL_MaximizeWindow(mWindow);
        }
    }
    bool IsMinimized() const
    {
        return (mWindow != nullptr) ? (SDL_GetWindowFlags(mWindow) & SDL_WINDOW_MINIMIZED) > 0 : false;
    }
    bool IsMaximized() const
    {
        return (mWindow != nullptr) ? (SDL_GetWindowFlags(mWindow) & SDL_WINDOW_MAXIMIZED) > 0 : false;
    }

    void SetSize(int width, int height)
    {
        if (mWindow != nullptr)
        {
            SDL_SetWindowSize(mWindow, width, height);
        }
    }
    int GetWidth() const
    {
        if (mWindow != nullptr)
		{
			int width, dummy;
            SDL_GetWindowSize(mWindow, &width, &dummy);
            return width;
        }
        else
        {
            return 0;
        }
    }
	int GetHeight() const
	{
		if (mWindow != nullptr)
		{
			int dummy, height;
			SDL_GetWindowSize(mWindow, &dummy, &height);
			return height;
		}
		else
		{
			return 0;
		}
	}
    void GetSize(int& width, int& height)
    {
        if (mWindow != nullptr)
        {
            SDL_GetWindowSize(mWindow, &width, &height);
        }
        else
        {
            width = 0;
            height = 0;
        }
    }

    void SetTitle(const char* title)
    {
        if (mWindow != nullptr)
        {
            SDL_SetWindowTitle(mWindow, title);
        }
    }
    const char* GetTitle() const
    {
        return (mWindow != nullptr) ? SDL_GetWindowTitle(mWindow) : "";
    }

    unsigned int GetFlags() const 
    { 
        return (mWindow != nullptr) ? SDL_GetWindowFlags(mWindow) : 0; 
    }

private:
    friend class Bgfx;

    SDL_Window* mWindow;
};