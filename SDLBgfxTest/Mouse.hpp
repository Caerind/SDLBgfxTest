#pragma once

#include "EngineIntegration.hpp"

#include "Window.hpp"

namespace NAMESPACE_NAME
{

class Mouse
{
public:
	// Call this at every frame beginning
	static void Refresh();
	static void HandleEvent(const SDL_Event& event);

	static bool SetRelativeMode(bool enabled);
	static bool IsRelativeMode();

	static void SetPositionGlobal(const Vector2i& mousePos);
	static Vector2i GetPositionGlobal();

	static void SetPositionCurrentWindow(const Vector2i& mousePos);
	static Vector2i GetPositionCurrentWindow();

	static Vector2i GetDeltaPosition();

	static I32 GetWheel();
	static I32 GetHorizontalWheel();

	static Window* GetCurrentWindow();

	enum class Button
	{
		Left,
		Middle,
		Right,
		X1,
		X2
	};
	static bool IsPressed(Button button);

private:
	static Mouse& GetInstance();

	Mouse();

	// NonCopyable & NonMovable
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	Mouse(Mouse&&) = delete;
	Mouse& operator=(Mouse&&) = delete;

	Vector2i mPosition;
	Vector2i mDelta;
	U32 mButtonMask;
	I32 mWheel;
	I32 mHorizontalWheel;
};

} // namespace NAMESPACE_NAME