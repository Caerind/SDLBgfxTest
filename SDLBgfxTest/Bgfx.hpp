#pragma once

#include <SDL.h>

#include <bgfx/bgfx.h>

#include "Window.hpp"

class Bgfx
{
public:
    static constexpr bgfx::ViewId kClearView = 0;
    static constexpr unsigned int kClearColor = 0x443355FF;

    static bool Init(Window& window);
    static bool IsInitialized();
    static bool Release();

private:
    static Bgfx& GetInstance();

    Bgfx();
    ~Bgfx();

    bool mInitialized;
};
