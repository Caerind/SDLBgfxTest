#pragma once

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

#if unix || __unix || __unix__ || __linux__ || linux || __linux || __FreeBSD__
	#define ENGINE_PLATFORM_X11
	#define ENGINE_PLATFORM_WAYLAND
#elif __APPLE__ || __MACH__
	#define ENGINE_PLATFORM_COCOA
	#define ENGINE_PLATFORM_UIKIT
#elif _WIN32 || _WIN64 
	#define ENGINE_PLATFORM_WINDOWS
	//#define ENGINE_PLATFORM_WINRT
#elif __ANDROID__
	#define ENGINE_PLATFORM_ANDROID
#endif

//#define ENGINE_PLATFORM_DIRECTFB
//#define ENGINE_PLATFORM_MIR
//#define ENGINE_PLATFORM_VIVANTE

#define ENGINE_DEBUG
#define ENGINE_IMGUI

#define NAMESPACE_NAME en

namespace NAMESPACE_NAME 
{
	using U16 = short int;
	using U32 = unsigned int;
	using I32 = int;
	using F32 = float;

	template <typename ... Args>
	void Debug(const char* msg, Args&& ... args)
	{
#ifdef ENGINE_DEBUG
		printf(msg, std::forward<Args>(args)...);
#endif // ENGINE_DEBUG
	}
}