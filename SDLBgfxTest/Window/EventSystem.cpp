#include "EventSystem.hpp"

#include "SDLWrapper.hpp"

namespace NAMESPACE_NAME
{

void EventSystem::Update()
{
	EventSystem& eventSystem = GetInstance();

	Mouse::Refresh();
	Keyboard::Refresh();
	Controller::Refresh();

	SDL_Event event;
	while (SDLWrapper::PollEvent(event))
	{
		eventSystem.HandleEvent(event);
	}

	for (EventButton& eventButton : eventSystem.mButtons)
	{
		switch (eventButton.type)
		{
		case ButtonType::KeyboardKey:
		{
			switch (eventButton.action)
			{
			case ButtonActionType::None:
			{
				eventButton.active = false;
				break;
			}
			case ButtonActionType::Hold:
			{
				eventButton.active = Keyboard::IsHold(static_cast<Keyboard::Key>(eventButton.buttonIdentifier));
				if (eventButton.active && eventButton.extraInfo > 0)
				{
					eventButton.active = Keyboard::AreModifiersHold(eventButton.extraInfo);
				}
				break;
			}
			case ButtonActionType::Pressed:
			{
				eventButton.active = Keyboard::IsPressed(static_cast<Keyboard::Key>(eventButton.buttonIdentifier));
				if (eventButton.active && eventButton.extraInfo > 0)
				{
					eventButton.active = Keyboard::AreModifiersHold(eventButton.extraInfo);
				}
				break;
			}
			case ButtonActionType::Released:
			{
				eventButton.active = Keyboard::IsReleased(static_cast<Keyboard::Key>(eventButton.buttonIdentifier));
				break;
			}
			default: assert(false); break;
			}
			break;
		}
		case ButtonType::MouseButton:
		{
			switch (eventButton.action)
			{
			case ButtonActionType::None:
			{
				eventButton.active = false;
				break;
			}
			case ButtonActionType::Hold:
			{
				eventButton.active = Mouse::IsHold(static_cast<Mouse::Button>(eventButton.buttonIdentifier));
				break;
			}
			case ButtonActionType::Pressed:
			{
				eventButton.active = Mouse::IsPressed(static_cast<Mouse::Button>(eventButton.buttonIdentifier));
				break;
			}
			case ButtonActionType::Released:
			{
				eventButton.active = Mouse::IsReleased(static_cast<Mouse::Button>(eventButton.buttonIdentifier));
				break;
			}
			default: assert(false); break;
			}
			break;
		}
		case ButtonType::JoystickButton:
		{
			switch (eventButton.action)
			{
			case ButtonActionType::None:
			{
				eventButton.active = false;
				break;
			}
			case ButtonActionType::Hold:
			{
				eventButton.active = Controller::IsButtonHold(eventButton.extraInfo, eventButton.buttonIdentifier);
				break;
			}
			case ButtonActionType::Pressed:
			{
				eventButton.active = Controller::IsButtonPressed(eventButton.extraInfo, eventButton.buttonIdentifier);
				break;
			}
			case ButtonActionType::Released:
			{
				eventButton.active = Controller::IsButtonReleased(eventButton.extraInfo, eventButton.buttonIdentifier);
				break;
			}
			default: assert(false); break;
			}
			break;
		}
		default: assert(false); break;
		}
	}
}

bool EventSystem::IsButtonActive(const char* name)
{
	return IsButtonActive(HashFct(name));
}

bool EventSystem::IsButtonActive(U32 hash)
{
	EventSystem& eventSystem = GetInstance();
	const U32 buttonCount = static_cast<U32>(eventSystem.mButtons.size());
	for (U32 i = 0; i < buttonCount; ++i)
	{
		if (eventSystem.mButtons[i].hash == hash)
		{
			return eventSystem.mButtons[i].active;
		}
	}
	return false;
}

U32 EventSystem::GetButtonCount()
{
	return static_cast<U32>(GetInstance().mButtons.size());
}

U32 EventSystem::AddButton(const char* name, Keyboard::Key key, ButtonActionType action, U32 modifiers /*= static_cast<U32>(Keyboard::Modifier::None)*/)
{
	EventButton eventButton;
#ifdef ENGINE_DEBUG
	eventButton.name = name;
#endif // ENGINE_DEBUG
	eventButton.hash = HashFct(name);
	eventButton.active = false;
	eventButton.action = action;
	eventButton.type = ButtonType::KeyboardKey;
	eventButton.buttonIdentifier = static_cast<U32>(key);
	eventButton.extraInfo = static_cast<U32>(modifiers);
	GetInstance().mButtons.push_back(eventButton);
	return eventButton.hash;
}

U32 EventSystem::AddButton(const char* name, Mouse::Button mouseButton, ButtonActionType action)
{
	EventButton eventButton;
#ifdef ENGINE_DEBUG
	eventButton.name = name;
#endif // ENGINE_DEBUG
	eventButton.hash = HashFct(name);
	eventButton.active = false;
	eventButton.action = action;
	eventButton.type = ButtonType::MouseButton;
	eventButton.buttonIdentifier = static_cast<U32>(mouseButton);
	eventButton.extraInfo = 0;
	GetInstance().mButtons.push_back(eventButton);
	return eventButton.hash;
}

U32 EventSystem::AddButton(const char* name, U32 controllerId, U32 buttonIndex, ButtonActionType action)
{
	EventButton eventButton;
#ifdef ENGINE_DEBUG
	eventButton.name = name;
#endif // ENGINE_DEBUG
	eventButton.hash = HashFct(name);
	eventButton.active = false;
	eventButton.action = action;
	eventButton.type = ButtonType::JoystickButton;
	eventButton.buttonIdentifier = buttonIndex;
	eventButton.extraInfo = controllerId;
	GetInstance().mButtons.push_back(eventButton);
	return eventButton.hash;
}

void EventSystem::RemoveButtonAtIndex(U32 index)
{
	EventSystem& eventSystem = GetInstance();
	eventSystem.mButtons.erase(eventSystem.mButtons.begin() + index);
}

void EventSystem::RemoveButton(const char* name)
{
	RemoveButton(HashFct(name));
}

void EventSystem::RemoveButton(U32 hash)
{
	EventSystem& eventSystem = GetInstance();
	const U32 buttonCount = static_cast<U32>(eventSystem.mButtons.size());
	for (U32 i = 0; i < buttonCount; ++i)
	{
		if (eventSystem.mButtons[i].hash == hash)
		{
			eventSystem.mButtons.erase(eventSystem.mButtons.begin() + i);
			return;
		}
	}
}

bool EventSystem::ShouldClose()
{
	return GetInstance().mShouldClose;
}

void EventSystem::ResetShouldClose()
{
	GetInstance().mShouldClose = false;
}

U32 EventSystem::HashFct(const char* name)
{
	if (name != nullptr)
	{
		U32 h = 89;
		while (*name != 0)
		{
			h = h * 33 + (*name++);
		}
		return h;
	}
	else
	{
		return 0;
	}
}

EventSystem& EventSystem::GetInstance()
{
	static EventSystem instance;
	return instance;
}

EventSystem::EventSystem()
	: mButtons()
	, mShouldClose(false)
{
}

void EventSystem::HandleEvent(const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEMOTION:
	case SDL_MOUSEWHEEL:
	case SDL_MOUSEBUTTONDOWN:
	{
		Mouse::HandleEvent(event);
		break;
	}
	case SDL_KEYDOWN:
	case SDL_KEYUP:
	case SDL_TEXTINPUT:
	{
		Keyboard::HandleEvent(event);
		break;
	}
	case SDL_JOYAXISMOTION:
	case SDL_JOYBUTTONDOWN:
	case SDL_JOYBUTTONUP:
	case SDL_JOYHATMOTION:
	case SDL_JOYBALLMOTION:
	{
		Controller::HandleEvent(event);
		break;
	}
	case SDL_QUIT:
	{
		mShouldClose = true;
		break;
	}
	case SDL_WINDOWEVENT:
	{
		switch (event.window.event)
		{
		case SDL_WINDOWEVENT_CLOSE:
		{
			mShouldClose = true;
			break;
		}
		}
		break;
	}
	}
}

} // namespace NAMESPACE_NAME