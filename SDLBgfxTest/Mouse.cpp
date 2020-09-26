#include "Mouse.hpp"

namespace NAMESPACE_NAME
{

void Mouse::Refresh()
{
	Mouse& mouse = GetInstance();

	int pX, pY;
	mouse.mButtonMask = SDL_GetMouseState(&pX, &pY);
	const I32 x = static_cast<I32>(pX);
	const I32 y = static_cast<I32>(pY);
	mouse.mDelta.x = x - mouse.mPosition.x;
	mouse.mDelta.y = y - mouse.mPosition.y;
	mouse.mPosition.x = x;
	mouse.mPosition.y = y;
	mouse.mWheel = 0;
	mouse.mHorizontalWheel = 0;
}

void Mouse::HandleEvent(const SDL_Event& event)
{
	Mouse& mouse = GetInstance();
	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		mouse.mButtonMask |= event.button.button;
	}
	else if (event.type == SDL_MOUSEWHEEL)
	{
		if (event.wheel.x > 0) mouse.mHorizontalWheel += 1;
		if (event.wheel.x < 0) mouse.mHorizontalWheel -= 1;
		if (event.wheel.y > 0) mouse.mWheel += 1;
		if (event.wheel.y < 0) mouse.mWheel -= 1;
	}
}

bool Mouse::SetRelativeMode(bool enabled)
{
	return SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE) >= 0;
}

bool Mouse::IsRelativeMode()
{
	return SDL_GetRelativeMouseMode();
}

void Mouse::SetPositionGlobal(const Vector2i& mousePos)
{
	SDL_WarpMouseGlobal(static_cast<int>(mousePos.x), static_cast<int>(mousePos.y));
}

Vector2i Mouse::GetPositionGlobal()
{
	int x, y;
	SDL_GetGlobalMouseState(&x, &y);
	return Vector2i(static_cast<I32>(x), static_cast<I32>(y));
}

void Mouse::SetPositionCurrentWindow(const Vector2i& mousePos)
{
	SDL_WarpMouseInWindow(nullptr, static_cast<int>(mousePos.x), static_cast<int>(mousePos.y));
	GetInstance().mPosition = mousePos;
}

Vector2i Mouse::GetPositionCurrentWindow()
{
	return GetInstance().mPosition;
}

Vector2i Mouse::GetDeltaPosition()
{
	return GetInstance().mDelta;
}

I32 Mouse::GetWheel()
{
	return GetInstance().mWheel;
}

I32 Mouse::GetHorizontalWheel()
{
	return GetInstance().mHorizontalWheel;
}

Window* Mouse::GetCurrentWindow()
{
	SDL_Window* window = SDL_GetMouseFocus();
	if (window != nullptr)
	{
		return Window::GetWindowFromSDL(window);
	}
	else
	{
		return nullptr;
	}
}

bool Mouse::IsPressed(Button button)
{
	Mouse& mouse = GetInstance();
	switch (button)
	{
	case Button::Left: return (SDL_BUTTON_LEFT & mouse.mButtonMask) > 0; break;
	case Button::Middle: return (SDL_BUTTON_MIDDLE & mouse.mButtonMask) > 0; break;
	case Button::Right: return (SDL_BUTTON_RIGHT & mouse.mButtonMask) > 0; break;
	case Button::X1: return (SDL_BUTTON_X1 & mouse.mButtonMask) > 0; break;
	case Button::X2: return (SDL_BUTTON_X2 & mouse.mButtonMask) > 0; break;
	}
	// Unhandled mouse button
	assert(false);
	return false;
}

Mouse& Mouse::GetInstance()
{
	static Mouse instance;
	return instance;
}

Mouse::Mouse()
	: mPosition(0, 0)
	, mDelta(0, 0)
	, mButtonMask(0)
	, mWheel(0)
	, mHorizontalWheel(0)
{
}

} // namespace NAMESPACE_NAME