#pragma once

#include "../EngineIntegration.hpp"

#include <vector>

#include <SDL.h>

namespace NAMESPACE_NAME
{

class Controller
{
public:
	static U32 GetJoystickCount() { return static_cast<U32>(SDL_NumJoysticks()); }
	static U32 GetHapticCount() { return static_cast<U32>(SDL_NumHaptics()); }

	static void HandleEvent(const SDL_Event& event)
	{
		if (event.type == SDL_JOYAXISMOTION)
		{
		}
		else if (event.type == SDL_JOYBUTTONDOWN || event.type == SDL_JOYBUTTONUP)
		{
		}
		else if (event.type == SDL_JOYHATMOTION)
		{
		}
		else if (event.type == SDL_JOYBALLMOTION)
		{
		}
	}

	static const char* GetControllerName(U32 controllerId)
	{
		Controller& controller = GetInstance();
		if (controllerId < static_cast<U32>(controller.mJoysticks.size()))
		{
			if (SDL_Joystick* joystick = controller.mJoysticks[controllerId].joystick)
			{
				return SDL_JoystickName(joystick);
			}
		}
		return "";
	}
	static U32 GetControllerAxisCount(U32 controllerId)
	{
		Controller& controller = GetInstance();
		if (controllerId < static_cast<U32>(controller.mJoysticks.size()))
		{
			if (SDL_Joystick* joystick = controller.mJoysticks[controllerId].joystick)
			{
				return SDL_JoystickNumAxes(joystick);
			}
		}
		return 0;
	}
	static U32 GetControllerBallCount(U32 controllerId)
	{
		Controller& controller = GetInstance();
		if (controllerId < static_cast<U32>(controller.mJoysticks.size()))
		{
			if (SDL_Joystick* joystick = controller.mJoysticks[controllerId].joystick)
			{
				return SDL_JoystickNumBalls(joystick);
			}
		}
		return 0;
	}
	static U32 GetControllerButtonCount(U32 controllerId)
	{
		Controller& controller = GetInstance();
		if (controllerId < static_cast<U32>(controller.mJoysticks.size()))
		{
			if (SDL_Joystick* joystick = controller.mJoysticks[controllerId].joystick)
			{
				return SDL_JoystickNumButtons(joystick);
			}
		}
		return 0;
	}
	static U32 GetControllerHatCount(U32 controllerId)
	{
		Controller& controller = GetInstance();
		if (controllerId < static_cast<U32>(controller.mJoysticks.size()))
		{
			if (SDL_Joystick* joystick = controller.mJoysticks[controllerId].joystick)
			{
				return SDL_JoystickNumHats(joystick);
			}
		}
		return 0;
	}

	static bool IsControllerHaptic(U32 controllerId)
	{
		Controller& controller = GetInstance();
		if (controllerId < static_cast<U32>(controller.mJoysticks.size()))
		{
			if (SDL_Joystick* joystick = controller.mJoysticks[controllerId].joystick)
			{
				return SDL_JoystickIsHaptic(joystick) && controller.mJoysticks[controllerId].haptic != nullptr;
			}
		}
		return false;
	}

	static bool Rumble(U32 controllerId, F32 strength, U32 durationMs)
	{
		Controller& controller = GetInstance();		
		if (controllerId < static_cast<U32>(controller.mJoysticks.size()))
		{
			if (SDL_Haptic* haptic = controller.mJoysticks[controllerId].haptic)
			{
				return SDL_HapticRumblePlay(haptic, strength, durationMs) >= 0;
			}
		}
		return false;
	}

private:
	static Controller& GetInstance() { static Controller instance; return instance; }

	Controller()
		: mJoysticks()
	{
		// Use the SDL event loop : Recommended by documentation
		SDL_JoystickEventState(SDL_ENABLE);

		const U32 joystickCount = GetJoystickCount();
		mJoysticks.reserve(joystickCount);
		for (U32 i = 0; i < joystickCount; ++i)
		{
			Joystick joystick;
			joystick.joystick = SDL_JoystickOpen(static_cast<int>(i));
			if (joystick.joystick != nullptr)
			{
				joystick.haptic = SDL_HapticOpenFromJoystick(joystick.joystick);
				if (joystick.haptic != nullptr)
				{
					SDL_HapticRumbleInit(joystick.haptic);
				}
			}
			mJoysticks.push_back(joystick);
		}
	}

	struct Joystick
	{
		SDL_Joystick* joystick;
		SDL_Haptic* haptic;
	};

	std::vector<Joystick> mJoysticks;
};

} // namespace NAMESPACE_NAME