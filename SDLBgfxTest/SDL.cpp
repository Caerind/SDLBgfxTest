#include "SDL.hpp"

#include <cassert>
#include <cstdio>

bool SDL::Init(unsigned int initFlags /*= SDL_INIT_EVERYTHING*/)
{
    SDL& sdl = GetInstance();

    assert(!sdl.mInitialized);

    sdl.mInitialized = SDL_Init(initFlags) >= 0;
    if (sdl.mInitialized)
    {
        sdl.mInitFlags = initFlags;
        return true;
    }
    else
    {
        sdl.mInitFlags = 0;
        return false;
    }
}

bool SDL::IsInitialized()
{
    return GetInstance().mInitialized;
}

bool SDL::Release()
{
    SDL& sdl = GetInstance();

    assert(sdl.mInitialized);

    SDL_Quit();
    sdl.mInitialized = false;

    return true;
}

bool SDL::PollEvent(SDL_Event& event)
{
    return GetInstance().mInitialized && SDL_PollEvent(&event) > 0;
}

bool SDL::WaitEvent(SDL_Event& event, int timeoutMs /*= -1*/)
{
    return GetInstance().mInitialized && SDL_WaitEventTimeout(&event, timeoutMs) > 0;
}

const char* SDL::GetError()
{
    return SDL_GetError();
}

unsigned int SDL::GetInitFlags()
{
    return GetInstance().mInitFlags;
}

SDL& SDL::GetInstance()
{
    static SDL instance;
    return instance;
}

SDL::SDL()
    : mInitialized(false)
    , mInitFlags(0)
{
}

SDL::~SDL()
{
    if (mInitialized)
    {
        printf("SDL hasn't been released\n");
        assert(false);
    }
}
