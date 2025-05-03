#pragma once

#include "KeyCode.h"
#include "Math/Constants.h"
#include "Math/Vectors.h"

#include <windows.h>

namespace MyEngine::App::Win32
{
/* Codes for 'physical' buttons on keyboard.
   * Names come from Qwerty keyboard.
     https://handmade.network/forums/t/2011-keyboard_inputs_-_scancodes,_raw_input,_text_input,_key_names
   */

class Keyboard
{
public:
   using Key = KeyCodeType;
};

template <class T>
class Keyboard_
	: public Keyboard
{
public:
	bool IsDown(Key c) const;

	Float2 GetWasdInput(float scale) const;
	Float2 GetArrowInput(Key left, Key right, Key up, Key down, float scale) const;
	Int2 GetArrowInput() const;
	int GetInput(Key positive, Key negative) const;

	static bool IsAzerty();
	static Key ScanCodeToVk(Key scanCode);
	static Key ScanCodeToVk(char scanCode);
	static Key VkToScanCode(Key virtualKey);
};

template <class T>
bool Keyboard_<T>::IsDown(Key c) const
{
	return reinterpret_cast<const T*>(this)->IsDown_(c);
}

template <class T>
Float2 Keyboard_<T>::GetWasdInput(float scale) const
{
	return GetArrowInput(
		ScanCodeToVk('Q'),
		ScanCodeToVk('D'),
		ScanCodeToVk('Z'),
		ScanCodeToVk('S'),
		scale);
}

template<class T>
inline Float2 Keyboard_<T>::GetArrowInput(Key left, Key right, Key up, Key down, float scale) const
{
	Float2 input;
	input.x = static_cast<float>(IsDown(right) - IsDown(left)) * scale;
	input.y = static_cast<float>(IsDown(up) - IsDown(down)) * scale;
	if (input.x != 0 && input.y != 0) input *= Constants::DIVSQR2;
	return input;
}

template <class T>
Int2 Keyboard_<T>::GetArrowInput() const
{
	return{
		IsDown(KeyCode::ARROW_RIGHT) - IsDown(KeyCode::ARROW_LEFT),
		IsDown(KeyCode::ARROW_UP) - IsDown(KeyCode::ARROW_DOWN) };
}

template <class T>
int Keyboard_<T>::GetInput(Key positive, Key negative) const
{
	return IsDown(positive) - IsDown(negative);
}
template<class T>
bool Keyboard_<T>::IsAzerty()
{
	const char qValue{ ScanCodeToVk(KeyCode::Q) };

	return qValue == KeyCode::A;
}
template<class T>
inline Keyboard::Key Keyboard_<T>::ScanCodeToVk(Key scanCode)
{
	return static_cast<Key>(
		MapVirtualKeyA(scanCode, MAPVK_VSC_TO_VK_EX));
}
template<class T>
inline Keyboard::Key Keyboard_<T>::ScanCodeToVk(char scanCode)
{
	return ScanCodeToVk(
		static_cast<Key>(scanCode));
}
template<class T>
inline Keyboard::Key Keyboard_<T>::VkToScanCode(Key virtualKey)
{
	return static_cast<Key>(
		MapVirtualKeyA(virtualKey, MAPVK_VK_TO_VSC_EX));
}
}
