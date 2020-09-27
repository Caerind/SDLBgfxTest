#include "Keyboard.hpp"

namespace NAMESPACE_NAME
{

void Keyboard::Refresh()
{
	Keyboard& keyboard = GetInstance();

	static constexpr U8 clearEvents = (1 << static_cast<U8>(KeyState::Pressed)) | (1 << static_cast<U8>(KeyState::Released));
	for (U32 i = 0; i < kKeyCount; ++i)
	{
		keyboard.mKeyStates[i] &= ~clearEvents;
	}
	keyboard.mInputCharacters[0] = '\0';
	keyboard.mInputCharacterCount = 0;
}

void Keyboard::HandleEvent(const SDL_Event& event)
{
	Keyboard& keyboard = GetInstance();
	const bool keyDown = event.type == SDL_KEYDOWN;
	const bool keyUp = event.type == SDL_KEYUP;
	if (keyDown || keyUp)
	{
		const Key key = GetKeyFromSDLKey(event.key.keysym.scancode);
		if (keyDown)
		{
			static constexpr U8 press = (1 << static_cast<U8>(KeyState::Hold)) | (1 << static_cast<U8>(KeyState::Pressed));
			keyboard.mKeyStates[static_cast<U32>(key)] |= press;
		}
		else // keyUp
		{
			static constexpr U8 release = (1 << static_cast<U8>(KeyState::Released));
			keyboard.mKeyStates[static_cast<U32>(key)] = release; // This also clear the hold & press
		}

		U8 modifier = 0;
		if (key == Key::LAlt || key == Key::RAlt)
		{
			modifier = static_cast<U8>(Modifier::Alt);
		}
		else if (key == Key::LCtrl || key == Key::RCtrl)
		{
			modifier = static_cast<U8>(Modifier::Control);
		}
		else if (key == Key::LShift || key == Key::RShift)
		{
			modifier = static_cast<U8>(Modifier::Shift);
		}
		else if (key == Key::LSystem || key == Key::RSystem)
		{
			modifier = static_cast<U8>(Modifier::System);
		}

		if (modifier > 0)
		{
			if (keyDown)
			{
				keyboard.mModifiers |= modifier;
			}
			else // keyUp
			{
				keyboard.mModifiers &= ~modifier;
			}
		}
	}
	else if (event.type == SDL_TEXTINPUT)
	{
		const U32 inputLength = strlen(event.text.text);
		if (keyboard.mInputCharacterCount + inputLength < kMaxInputCharacters)
		{
			strcat(keyboard.mInputCharacters, event.text.text);
			keyboard.mInputCharacterCount += inputLength;
		}
		else
		{
			// Maximum amount of input characters reached
			// Increase Keyboard::kMaxInputCharacters or use less input characters
			assert(false);
		}
	}
}

bool Keyboard::IsHold(Key key)
{
	static constexpr U8 hold = 1 << static_cast<U8>(KeyState::Hold);
	return (GetInstance().mKeyStates[static_cast<U32>(key)] & hold) > 0;
}

bool Keyboard::IsPressed(Key key)
{
	static constexpr U8 pressed = 1 << static_cast<U8>(KeyState::Pressed);
	return (GetInstance().mKeyStates[static_cast<U32>(key)] & pressed) > 0;
}

bool Keyboard::IsReleased(Key key)
{
	static constexpr U8 released = 1 << static_cast<U8>(KeyState::Released);
	return (GetInstance().mKeyStates[static_cast<U32>(key)] & released) > 0;
}

bool Keyboard::IsAltHold()
{
	return (GetInstance().mModifiers & static_cast<U32>(Modifier::Alt)) > 0;
}

bool Keyboard::IsControlHold()
{
	return (GetInstance().mModifiers & static_cast<U32>(Modifier::Control)) > 0;
}

bool Keyboard::IsShiftHold()
{
	return (GetInstance().mModifiers & static_cast<U32>(Modifier::Shift)) > 0;
}

bool Keyboard::IsSystemHold()
{
	return (GetInstance().mModifiers & static_cast<U32>(Modifier::System)) > 0;
}

const char* Keyboard::GetInputCharacters()
{
	return GetInstance().mInputCharacters;
}

U32 Keyboard::GetInputCharacterCount()
{
	return GetInstance().mInputCharacterCount;
}

const char* Keyboard::GetKeyName(Key key)
{
#define KeyValueToKeyName(key) case Key::##key: return #key; break;
	switch (key)
	{
		case Key::A: return "A"; break;
		case Key::B: return "B"; break;
		case Key::C: return "C"; break;
		case Key::D: return "D"; break;
		case Key::E: return "E"; break;
		case Key::F: return "F"; break;
		case Key::G: return "G"; break;
		case Key::H: return "H"; break;
		case Key::I: return "I"; break;
		case Key::J: return "J"; break;
		case Key::K: return "K"; break;
		case Key::L: return "L"; break;
		case Key::M: return "M"; break;
		case Key::N: return "N"; break;
		case Key::O: return "O"; break;
		case Key::P: return "P"; break;
		case Key::Q: return "Q"; break;
		case Key::R: return "R"; break;
		case Key::S: return "S"; break;
		case Key::T: return "T"; break;
		case Key::U: return "U"; break;
		case Key::V: return "V"; break;
		case Key::W: return "W"; break;
		case Key::X: return "X"; break;
		case Key::Y: return "Y"; break;
		case Key::Z: return "Z"; break;
		case Key::Num0: return "0"; break;
		case Key::Num1: return "1"; break;
		case Key::Num2: return "2"; break;
		case Key::Num3: return "3"; break;
		case Key::Num4: return "4"; break;
		case Key::Num5: return "5"; break;
		case Key::Num6: return "6"; break;
		case Key::Num7: return "7"; break;
		case Key::Num8: return "8"; break;
		case Key::Num9: return "9"; break;
		case Key::Return: return "Return"; break;
		case Key::Escape: return "Escape"; break;
		case Key::Backspace: return "Backspace"; break;
		case Key::Tab: return "Return"; break;
		case Key::Space: return "Return"; break;
		case Key::LCtrl: return "Return"; break;
		case Key::LShift: return "LShift"; break;
		case Key::LAlt: return "LAlt"; break;
		case Key::LSystem: return "LSystem"; break;
		case Key::RCtrl: return "RCtrl"; break;
		case Key::RShift: return "RShift"; break;
		case Key::RAlt: return "RAlt"; break;
		case Key::RSystem: return "RSystem"; break;
		case Key::Menu: return "Menu"; break;
		case Key::LBracket: return "LBracket"; break;
		case Key::RBracket: return "RBracket"; break;
		case Key::Semicolon: return "Semicolon"; break;
		case Key::Comma: return "Comma"; break;
		case Key::Period: return "Period"; break;
		case Key::Slash: return "Slash"; break;
		case Key::Backslash: return "Backslash"; break;
		case Key::Tilde: return "Tilde"; break;
		case Key::Equal: return "Equal"; break;
		case Key::Hyphen: return "Hyphen"; break;
		case Key::PageUp: return "PageUp"; break;
		case Key::PageDown: return "PageDown"; break;
		case Key::End: return "End"; break;
		case Key::Home: return "Home"; break;
		case Key::Insert: return "Insert"; break;
		case Key::Delete: return "Delete"; break;
		case Key::Left: return "Left"; break;
		case Key::Right: return "Right"; break;
		case Key::Up: return "Up"; break;
		case Key::Down: return "Down"; break;
		case Key::F1: return "F1"; break;
		case Key::F2: return "F2"; break;
		case Key::F3: return "F3"; break;
		case Key::F4: return "F4"; break;
		case Key::F5: return "F5"; break;
		case Key::F6: return "F6"; break;
		case Key::F7: return "F7"; break;
		case Key::F8: return "F8"; break;
		case Key::F9: return "F9"; break;
		case Key::F10: return "F10"; break;
		case Key::F11: return "F11"; break;
		case Key::F12: return "F12"; break;
		default: break;
	}
#undef KeyValueToKeyName
	return "Unknown";
}

Keyboard::Key Keyboard::GetKeyFromSDLKey(SDL_Scancode scancode)
{
	switch (scancode)
	{
    case SDL_SCANCODE_A: return Key::A; break;
    case SDL_SCANCODE_B: return Key::B; break;
    case SDL_SCANCODE_C: return Key::C; break;
    case SDL_SCANCODE_D: return Key::D; break;
    case SDL_SCANCODE_E: return Key::E; break;
    case SDL_SCANCODE_F: return Key::F; break;
    case SDL_SCANCODE_G: return Key::G; break;
    case SDL_SCANCODE_H: return Key::H; break;
    case SDL_SCANCODE_I: return Key::I; break;
    case SDL_SCANCODE_J: return Key::J; break;
    case SDL_SCANCODE_K: return Key::K; break;
    case SDL_SCANCODE_L: return Key::L; break;
    case SDL_SCANCODE_M: return Key::M; break;
    case SDL_SCANCODE_N: return Key::N; break;
    case SDL_SCANCODE_O: return Key::O; break;
    case SDL_SCANCODE_P: return Key::P; break;
    case SDL_SCANCODE_Q: return Key::Q; break;
    case SDL_SCANCODE_R: return Key::R; break;
    case SDL_SCANCODE_S: return Key::S; break;
    case SDL_SCANCODE_T: return Key::T; break;
    case SDL_SCANCODE_U: return Key::U; break;
    case SDL_SCANCODE_V: return Key::V; break;
    case SDL_SCANCODE_W: return Key::W; break;
    case SDL_SCANCODE_X: return Key::X; break;
    case SDL_SCANCODE_Y: return Key::Y; break;
    case SDL_SCANCODE_Z: return Key::Z; break;
    case SDL_SCANCODE_0: return Key::Num0; break;
    case SDL_SCANCODE_1: return Key::Num1; break;
    case SDL_SCANCODE_2: return Key::Num2; break;
    case SDL_SCANCODE_3: return Key::Num3; break;
    case SDL_SCANCODE_4: return Key::Num4; break;
    case SDL_SCANCODE_5: return Key::Num5; break;
    case SDL_SCANCODE_6: return Key::Num6; break;
    case SDL_SCANCODE_7: return Key::Num7; break;
	case SDL_SCANCODE_8: return Key::Num8; break;
	case SDL_SCANCODE_9: return Key::Num9; break;
	case SDL_SCANCODE_RETURN: return Key::Return; break;
	case SDL_SCANCODE_ESCAPE: return Key::Escape; break;
	case SDL_SCANCODE_BACKSPACE: return Key::Backspace; break;
	case SDL_SCANCODE_TAB: return Key::Tab; break;
	case SDL_SCANCODE_SPACE: return Key::Space; break;
	case SDL_SCANCODE_LCTRL: return Key::LCtrl; break;
	case SDL_SCANCODE_LSHIFT: return Key::LShift; break;
	case SDL_SCANCODE_LALT: return Key::LAlt; break;
	case SDL_SCANCODE_LGUI: return Key::LSystem; break;
	case SDL_SCANCODE_RCTRL: return Key::RCtrl; break;
	case SDL_SCANCODE_RSHIFT: return Key::RShift; break;
	case SDL_SCANCODE_RALT: return Key::RAlt; break;
	case SDL_SCANCODE_RGUI: return Key::RSystem; break;
	case SDL_SCANCODE_MENU: return Key::Menu; break;
	case SDL_SCANCODE_LEFTBRACKET: return Key::LBracket; break;
	case SDL_SCANCODE_RIGHTBRACKET: return Key::RBracket; break;
	case SDL_SCANCODE_SEMICOLON: return Key::Semicolon; break;
	case SDL_SCANCODE_COMMA: return Key::Comma; break;
	case SDL_SCANCODE_PERIOD: return Key::Period; break;
	case SDL_SCANCODE_SLASH: return Key::Slash; break;
	case SDL_SCANCODE_BACKSLASH: return Key::Backslash; break;
	case SDL_SCANCODE_GRAVE: return Key::Tilde; break;
	case SDL_SCANCODE_EQUALS: return Key::Equal; break;
	case SDL_SCANCODE_MINUS: return Key::Hyphen; break;
	case SDL_SCANCODE_PAGEUP: return Key::PageUp; break;
	case SDL_SCANCODE_PAGEDOWN: return Key::PageDown; break;
	case SDL_SCANCODE_END: return Key::End; break;
	case SDL_SCANCODE_HOME: return Key::Home; break;
	case SDL_SCANCODE_INSERT: return Key::Insert; break;
	case SDL_SCANCODE_DELETE: return Key::Delete; break;
	case SDL_SCANCODE_LEFT: return Key::Left; break;
	case SDL_SCANCODE_RIGHT: return Key::Right; break;
	case SDL_SCANCODE_UP: return Key::Up; break;
	case SDL_SCANCODE_DOWN: return Key::Down; break;
	case SDL_SCANCODE_PAUSE: return Key::Pause; break;
	case SDL_SCANCODE_F1: return Key::F1; break;
	case SDL_SCANCODE_F2: return Key::F2; break;
	case SDL_SCANCODE_F3: return Key::F3; break;
	case SDL_SCANCODE_F4: return Key::F4; break;
	case SDL_SCANCODE_F5: return Key::F5; break;
	case SDL_SCANCODE_F6: return Key::F6; break;
	case SDL_SCANCODE_F7: return Key::F7; break;
	case SDL_SCANCODE_F8: return Key::F8; break;
	case SDL_SCANCODE_F9: return Key::F9; break;
	case SDL_SCANCODE_F10: return Key::F10; break;
	case SDL_SCANCODE_F11: return Key::F11; break;
	case SDL_SCANCODE_F12: return Key::F12; break;
    default: break;
	}
    return Key::Unknown;
}

Keyboard& Keyboard::GetInstance()
{
	static Keyboard instance;
	return instance;
}

Keyboard::Keyboard()
	: mKeyStates()
	, mModifiers(0)
	, mInputCharacters()
	, mInputCharacterCount()
{
}

} // namespace NAMESPACE_NAME