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

	template <typename T>
	struct Rect
	{
		Rect() : left(T(0)), top(T(0)), width(T(0)), height(T(0)) {}
		Rect(T _left, T _top, T _width, T _height) : left(_left), top(_top), width(_width), height(_height) {}

		bool operator==(const Rect& other) const { return left == other.left && top == other.top && width == other.width && height == other.height; }
		bool operator!=(const Rect& other) const { return !operator==(other); }

		T left;
		T top;
		T width;
		T height;
	};
	using Rectf = Rect<F32>;
	using Rectu = Rect<U32>;
	using Recti = Rect<I32>;

	struct Color
	{
		Color() : r(255), g(255), b(255), a(255) {}
		Color(U8 _r, U8 _g, U8 _b, U8 _a = 255) : r(_r), g(_g), b(_b), a(_a) {}

		U8 r;
		U8 g;
		U8 b;
		U8 a;

		U32 toABGR() const
		{
			return (U32(r) << 0) + (U32(g) << 8) + (U32(b) << 16) + (U32(a) << 24);
		}
		U32 toRGBA() const
		{
			return (U32(r) << 24) + (U32(g) << 16) + (U32(b) << 8) + (U32(a) << 0);
		}
	};

	template <typename ... Args>
	void Debug(const char* msg, Args&& ... args)
	{
#ifdef ENGINE_DEBUG
		printf(msg, std::forward<Args>(args)...);
#endif // ENGINE_DEBUG
	}
}