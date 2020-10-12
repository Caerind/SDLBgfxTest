#pragma once

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <utility>

#if unix || __unix || __unix__ || __linux__ || linux || __linux || __FreeBSD__
	#define ENGINE_PLATFORM_LINUX
#elif __APPLE__ || __MACH__
	#define ENGINE_PLATFORM_APPLE
#elif _WIN32 || _WIN64 
	#define ENGINE_PLATFORM_WINDOWS
#elif __ANDROID__
	#define ENGINE_PLATFORM_ANDROID
#endif

#define ENGINE_DEBUG
#define ENGINE_IMGUI

#define NAMESPACE_NAME en

#define ENGINE_DEFAULT_UP NAMESPACE_NAME::Vector3f::UnitY()
#define ENGINE_DEFAULT_HANDEDNESS NAMESPACE_NAME::Math::Handedness::Right

namespace NAMESPACE_NAME
{
	
using U8 = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;
using I8 = std::int8_t;
using I16 = std::int16_t;
using I32 = std::int32_t;
using I64 = std::int64_t;
using F32 = float;
using F64 = double;

} // NAMESPACE_NAME