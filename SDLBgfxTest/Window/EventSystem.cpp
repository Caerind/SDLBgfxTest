#include "EventSystem.hpp"

#include "SDLWrapper.hpp"

namespace NAMESPACE_NAME
{

void EventSystem::Update()
{
	EventSystem& eventSystem = GetInstance();

	Mouse::Refresh();
	Keyboard::Refresh();

	SDL_Event event;
	while (SDLWrapper::PollEvent(event))
	{
		eventSystem.HandleEvent(event);
	}

	for (EventKey& eventKey : eventSystem.mKeys)
	{
		switch (eventKey.action)
		{
		case ButtonActionType::None: 
			eventKey.active = false; 
			break;
		case ButtonActionType::Hold: 
			eventKey.active = Keyboard::IsHold(eventKey.key);
			if (eventKey.active && eventKey.modifiers > 0)
			{
				eventKey.active = Keyboard::AreModifiersHold(eventKey.modifiers);
			}
			break;
		case ButtonActionType::Pressed: 
			eventKey.active = Keyboard::IsPressed(eventKey.key);
			if (eventKey.active && eventKey.modifiers > 0)
			{
				eventKey.active = Keyboard::AreModifiersHold(eventKey.modifiers);
			}
			break;
		case ButtonActionType::Released: 
			eventKey.active = Keyboard::IsReleased(eventKey.key);
			break;
		default: 
			assert(false); 
			break;
		}
	}

	for (EventMouseButton& eventMouseButton : eventSystem.mMouseButtons)
	{
		switch (eventMouseButton.action)
		{
		case ButtonActionType::None: 
			eventMouseButton.active = false; 
			break;
		case ButtonActionType::Hold: 
			eventMouseButton.active = Mouse::IsHold(eventMouseButton.button);
			break;
		case ButtonActionType::Pressed: 
			eventMouseButton.active = Mouse::IsPressed(eventMouseButton.button);
			break;
		case ButtonActionType::Released: 
			eventMouseButton.active = Mouse::IsReleased(eventMouseButton.button);
			break;
		default: 
			assert(false); 
			break;
		}
	}
}

bool EventSystem::IsKeyActive(const char* name)
{
	return IsKeyActive(HashFct(name));
}

bool EventSystem::IsKeyActive(U32 hash)
{
	EventSystem& eventSystem = GetInstance();
	const U32 keyCount = static_cast<U32>(eventSystem.mKeys.size());
	for (U32 i = 0; i < keyCount; ++i)
	{
		if (eventSystem.mKeys[i].hash == hash)
		{
			return eventSystem.mKeys[i].active;
		}
	}
	return false;
}

U32 EventSystem::GetKeyCount()
{
	return static_cast<U32>(GetInstance().mKeys.size());
}

U32 EventSystem::AddKey(const char* name, Keyboard::Key key, ButtonActionType action, U32 modifiers)
{
	EventKey eventKey;
#ifdef ENGINE_DEBUG
	eventKey.name = name;
#endif // ENGINE_DEBUG
	eventKey.hash = HashFct(name);
	eventKey.active = false;
	eventKey.key = key;
	eventKey.action = action;
	eventKey.modifiers = modifiers;
	GetInstance().mKeys.push_back(eventKey);
	return eventKey.hash;
}

void EventSystem::RemoveKeyAtIndex(U32 index)
{
	EventSystem& eventSystem = GetInstance();
	eventSystem.mKeys.erase(eventSystem.mKeys.begin() + index);
}

void EventSystem::RemoveKey(const char* name)
{
	RemoveKey(HashFct(name));
}

void EventSystem::RemoveKey(U32 hash)
{
	EventSystem& eventSystem = GetInstance();
	const U32 keyCount = static_cast<U32>(eventSystem.mKeys.size());
	for (U32 i = 0; i < keyCount; ++i)
	{
		if (eventSystem.mKeys[i].hash == hash)
		{
			eventSystem.mKeys.erase(eventSystem.mKeys.begin() + i);
			return;
		}
	}
}

bool EventSystem::IsMouseButtonActive(const char* name)
{
	return IsMouseButtonActive(HashFct(name));
}

bool EventSystem::IsMouseButtonActive(U32 hash)
{
	EventSystem& eventSystem = GetInstance();
	const U32 mouseButtonCount = static_cast<U32>(eventSystem.mMouseButtons.size());
	for (U32 i = 0; i < mouseButtonCount; ++i)
	{
		if (eventSystem.mMouseButtons[i].hash == hash)
		{
			return eventSystem.mMouseButtons[i].active;
		}
	}
	return false;
}

U32 EventSystem::GetMouseButtonCount()
{
	return static_cast<U32>(GetInstance().mMouseButtons.size());
}

U32 EventSystem::AddMouseButton(const char* name, Mouse::Button mouseButton, ButtonActionType action)
{
	EventMouseButton eventMouseButton;
#ifdef ENGINE_DEBUG
	eventMouseButton.name = name;
#endif // ENGINE_DEBUG
	eventMouseButton.hash = HashFct(name);
	eventMouseButton.active = false;
	eventMouseButton.button = mouseButton;
	eventMouseButton.action = action;
	GetInstance().mMouseButtons.push_back(eventMouseButton);
	return eventMouseButton.hash;
}

void EventSystem::RemoveMouseButtonAtIndex(U32 index)
{
	EventSystem& eventSystem = GetInstance();
	eventSystem.mMouseButtons.erase(eventSystem.mMouseButtons.begin() + index);
}

void EventSystem::RemoveMouseButton(const char* name)
{
	RemoveMouseButton(HashFct(name));
}

void EventSystem::RemoveMouseButton(U32 hash)
{
	EventSystem& eventSystem = GetInstance();
	const U32 mouseButtonCount = static_cast<U32>(eventSystem.mMouseButtons.size());
	for (U32 i = 0; i < mouseButtonCount; ++i)
	{
		if (eventSystem.mMouseButtons[i].hash == hash)
		{
			eventSystem.mMouseButtons.erase(eventSystem.mMouseButtons.begin() + i);
			return;
		}
	}
}

void EventSystem::SetMouseMoveThreshold(F32 mouseMoveThreshold)
{
	GetInstance().mMouseMoveTreshold = mouseMoveThreshold;
}

F32 EventSystem::GetMouseMoveThreshold()
{
	return GetInstance().mMouseMoveTreshold;
}

bool EventSystem::HasMouseMoved()
{
	const Vector2i mouseDelta = Mouse::GetDeltaPosition();
	const F32 mouseThreshold = GetInstance().mMouseMoveTreshold;
	return static_cast<F32>(mouseDelta.GetSquaredLength()) >= mouseThreshold * mouseThreshold;
}

Vector2f EventSystem::GetMouseDelta()
{
	return Vector2f(Mouse::GetDeltaPosition());
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
	: mKeys()
	, mMouseButtons()
	, mMouseMoveTreshold(1.0f)
	, mShouldClose(false)
{
}

void EventSystem::HandleEvent(const SDL_Event& event)
{
	switch (event.type)
	{
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