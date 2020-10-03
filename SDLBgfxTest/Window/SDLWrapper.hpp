#pragma once

#include "../EngineIntegration.hpp"

#include <SDL.h>

namespace NAMESPACE_NAME
{

class SDLWrapper
{
public:
	static bool Init();
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