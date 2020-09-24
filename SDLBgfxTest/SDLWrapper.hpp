#pragma once

#include <SDL.h>

#include "EngineIntegration.hpp"

// TODO : https://alexandre-laurent.developpez.com/tutoriels/sdl-2/les-evenements/

namespace NAMESPACE_NAME
{

class SDLWrapper
{
public:
	static bool Init(U32 initFlags = SDL_INIT_EVERYTHING);
	static bool IsInitialized();
	static bool Release();

	static bool PollEvent(SDL_Event& event);
	static bool WaitEvent(SDL_Event& event, I32 timeoutMs = -1);

	static const char* GetError();

	static U32 GetInitFlags();

private:
	static SDLWrapper& GetInstance();

	SDLWrapper();
	~SDLWrapper();

	bool mInitialized;
	U32 mInitFlags;
};

} // namespace NAMESPACE_NAME