#pragma once

#include <SDL.h>

// TODO : https://alexandre-laurent.developpez.com/tutoriels/sdl-2/creer-premieres-fenetres

class Window
{
public:
    Window();

    Window(const char* name, int width, int height);

    ~Window();

    bool Create(const char* name, int width, int height, unsigned int flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    void Close();

    bool IsOpen() const;

    void SetVisible(bool visible);
    bool IsVisible() const;

    void Minimize();
    void Maximize();
    bool IsMinimized() const;
    bool IsMaximized() const;

    void SetSize(int width, int height);
    int GetWidth() const;
    int GetHeight() const;
    void GetSize(int& width, int& height);

    void SetTitle(const char* title);
    const char* GetTitle() const;

    unsigned int GetFlags() const;

private:
    friend class Bgfx;

    SDL_Window* mWindow;
};