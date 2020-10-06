#pragma once

#include "../EngineIntegration.hpp"

#include <vector>
#ifdef ENGINE_DEBUG
#include <string>
#endif // ENGINE_DEBUG

#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Controller.hpp"

namespace NAMESPACE_NAME
{

class EventSystem
{
public:
	enum class ButtonActionType
	{
		None,
		Pressed,
		Released,
		Hold
	};

	enum class ButtonType
	{
		KeyboardKey,
		MouseButton,
		JoystickButton
	};

	class EventBase
	{
	protected:
		EventBase() = default;

#ifdef ENGINE_DEBUG
		std::string name{};
#endif // ENGINE_DEBUG
		U32 hash{ 0 };
		bool active{ false };
	};

	class EventButton : public EventBase
	{
	public:
		ButtonType type{ButtonType::KeyboardKey};
		U32 buttonIdentifier{ 0 }; // Key, MouseButton or JoystickButtonIndex
		U32 extraInfo{ 0 }; // KeyModifiers, nothing or ControllerId
		ButtonActionType action{ ButtonActionType::None };
	private:
		friend class EventSystem;
		EventButton() = default;
	};

public:
	static void Update();

	static bool IsButtonActive(const char* name);
	static bool IsButtonActive(U32 hash);
	static U32 GetButtonCount();
	static U32 AddButton(const char* name, Keyboard::Key key, ButtonActionType action, U32 modifiers = static_cast<U32>(Keyboard::Modifier::None));
	static U32 AddButton(const char* name, Mouse::Button mouseButton, ButtonActionType action);
	static U32 AddButton(const char* name, U32 controllerId, U32 buttonIndex, ButtonActionType action);
	static void RemoveButtonAtIndex(U32 index);
	static void RemoveButton(const char* name);
	static void RemoveButton(U32 hash);

	static bool ShouldClose();
	static void ResetShouldClose();

private:
	static U32 HashFct(const char* name);

	static EventSystem& GetInstance();

	EventSystem();
	void HandleEvent(const SDL_Event& event);

	// NonCopyable & NonMovable
	EventSystem(const EventSystem&) = delete;
	EventSystem& operator=(const EventSystem&) = delete;
	EventSystem(Mouse&&) = delete;
	EventSystem& operator=(EventSystem&&) = delete;

	std::vector<EventButton> mButtons;
	bool mShouldClose;
};

} // namespace NAMESPACE_NAME