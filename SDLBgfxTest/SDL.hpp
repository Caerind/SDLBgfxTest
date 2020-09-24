#pragma once

#include <SDL.h>

// TODO : https://alexandre-laurent.developpez.com/tutoriels/sdl-2/les-evenements/

class SDL
{
public:
    static bool Init(unsigned int initFlags = SDL_INIT_EVERYTHING);
    static bool IsInitialized();
    static bool Release();

    static bool PollEvent(SDL_Event& event);
    static bool WaitEvent(SDL_Event& event, int timeoutMs = -1);

    static const char* GetError();

    static unsigned int GetInitFlags();

private:
    static SDL& GetInstance();

    SDL();
    ~SDL();

	bool mInitialized;
	unsigned int mInitFlags;
};
