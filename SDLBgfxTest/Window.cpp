#include "Window.hpp"

namespace NAMESPACE_NAME
{

Window::Window()
    : mWindow(nullptr)
{
}

Window::Window(const char* name, I32 width, I32 height)
    : Window()
{
    Create(name, width, height);
}

Window::~Window()
{
    Close();
}

bool Window::Create(const char* name, I32 width, I32 height, U32 flags /*= SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE*/)
{
    mWindow = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
    return mWindow != nullptr;
}

void Window::Close()
{
    if (mWindow != nullptr)
    {
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }
}

bool Window::IsOpen() const
{
    return mWindow != nullptr;
}

void Window::SetVisible(bool visible)
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

bool Window::IsVisible() const
{
    return (mWindow != nullptr) ? (SDL_GetWindowFlags(mWindow) & SDL_WINDOW_SHOWN) > 0 : false;
}

void Window::Minimize()
{
    if (mWindow != nullptr)
    {
        SDL_MinimizeWindow(mWindow);
    }
}

void Window::Maximize()
{
    if (mWindow != nullptr)
    {
        SDL_MaximizeWindow(mWindow);
    }
}

bool Window::IsMinimized() const
{
    return (mWindow != nullptr) ? (SDL_GetWindowFlags(mWindow) & SDL_WINDOW_MINIMIZED) > 0 : false;
}

bool Window::IsMaximized() const
{
    return (mWindow != nullptr) ? (SDL_GetWindowFlags(mWindow) & SDL_WINDOW_MAXIMIZED) > 0 : false;
}

void Window::SetSize(I32 width, I32 height)
{
    if (mWindow != nullptr)
    {
        SDL_SetWindowSize(mWindow, width, height);
    }
}

I32 Window::GetWidth() const
{
    if (mWindow != nullptr)
    {
        I32 width, dummy;
        SDL_GetWindowSize(mWindow, &width, &dummy);
        return width;
    }
    else
    {
        return 0;
    }
}

I32 Window::GetHeight() const
{
    if (mWindow != nullptr)
    {
        I32 dummy, height;
        SDL_GetWindowSize(mWindow, &dummy, &height);
        return height;
    }
    else
    {
        return 0;
    }
}

void Window::GetSize(I32& width, I32& height)
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

void Window::SetTitle(const char* title)
{
    if (mWindow != nullptr)
    {
        SDL_SetWindowTitle(mWindow, title);
    }
}

const char* Window::GetTitle() const
{
    return (mWindow != nullptr) ? SDL_GetWindowTitle(mWindow) : "";
}

U32 Window::GetFlags() const
{
    return (mWindow != nullptr) ? SDL_GetWindowFlags(mWindow) : 0;
}

} // namespace NAMESPACE_NAME