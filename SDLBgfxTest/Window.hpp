#pragma once

#include <SDL.h>

#include "EngineIntegration.hpp"

// TODO : https://alexandre-laurent.developpez.com/tutoriels/sdl-2/creer-premieres-fenetres

namespace NAMESPACE_NAME
{

class Window
{
public:
    Window();
    Window(const char* name, I32 width, I32 height);
    ~Window();

    bool Create(const char* name, I32 width, I32 height, U32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    void Close();

    bool IsOpen() const;

    void SetVisible(bool visible);
    bool IsVisible() const;

    void Minimize();
    void Maximize();
    bool IsMinimized() const;
    bool IsMaximized() const;

    void SetSize(I32 width, I32 height);
    I32 GetWidth() const;
    I32 GetHeight() const;
    void GetSize(I32& width, I32& height);

    void SetTitle(const char* title);
    const char* GetTitle() const;

    U32 GetFlags() const;

private:
    friend class BgfxWrapper;

    SDL_Window* mWindow;
};

} // namespace NAMESPACE_NAME