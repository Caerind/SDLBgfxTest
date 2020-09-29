#include "SDLWrapper.hpp"

namespace NAMESPACE_NAME
{

bool SDLWrapper::Init()
{
    SDLWrapper& sdl = GetInstance();

    assert(!sdl.mInitialized);

    U32 initFlags = SDL_INIT_VIDEO | SDL_INIT_EVENTS; // SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER | SDL_INIT_SENSOR

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

bool SDLWrapper::IsInitialized()
{
    return GetInstance().mInitialized;
}

bool SDLWrapper::Release()
{
    SDLWrapper& sdl = GetInstance();

    assert(sdl.mInitialized);

    SDL_Quit();
    sdl.mInitialized = false;

    return true;
}

bool SDLWrapper::PollEvent(SDL_Event& event)
{
    return GetInstance().mInitialized && SDL_PollEvent(&event) > 0;
}

bool SDLWrapper::WaitEvent(SDL_Event& event, I32 timeoutMs /*= -1*/)
{
    return GetInstance().mInitialized && SDL_WaitEventTimeout(&event, timeoutMs) > 0;
}

const char* SDLWrapper::GetError()
{
    return SDL_GetError();
}

U32 SDLWrapper::GetInitFlags()
{
    return GetInstance().mInitFlags;
}

SDLWrapper& SDLWrapper::GetInstance()
{
    static SDLWrapper instance;
    return instance;
}

SDLWrapper::SDLWrapper()
    : mInitialized(false)
    , mInitFlags(0)
{
}

SDLWrapper::~SDLWrapper()
{
    assert(!mInitialized);
}

} // namespace NAMESPACE_NAME