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
	using U8 = char;
	using U16 = short int;
	using U32 = unsigned int;
	using U64 = unsigned long long;
	using I32 = int;
	using F32 = float;

	template <typename T>
	struct Vector2
	{
		Vector2() : x(T(0)), y(T(0)) {}
		Vector2(T s) : x(s), y(s) {}
		Vector2(T _x, T _y) : x(_x), y(_y) {}

		bool operator==(const Vector2& other) const { return x == other.x && y == other.y; }
		bool operator!=(const Vector2& other) const { return !operator==(other); }

		T x;
		T y;
	};
	using Vector2f = Vector2<F32>;
	using Vector2u = Vector2<U32>;
	using Vector2i = Vector2<I32>;

	template <typename T>
	struct Vector3
	{
		Vector3() : x(T(0)), y(T(0)), z(T(0)) {}
		Vector3(T s) : x(s), y(s), z(s) {}
		Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

		bool operator==(const Vector3& other) const { return x == other.x && y == other.y && z == other.z; }
		bool operator!=(const Vector3& other) const { return !operator==(other); }

		T x;
		T y;
		T z;
	};
	using Vector3f = Vector3<F32>;
	using Vector3u = Vector3<U32>;
	using Vector3i = Vector3<I32>;

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