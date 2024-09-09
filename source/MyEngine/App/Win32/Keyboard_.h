#pragma once
#include "KeyCodes.h"
#include "Math/Constants.h"
#include "Math/Vectors.h"
#include <windows.h>

namespace MyEngine
{
namespace App
{
namespace Win32
{
template <class T>
class Keyboard_
{
public:
	bool IsDown(char c) const;

	Float2 GetWasdInput(float scale) const;
	Float2 GetArrowInput(char left, char right, char up, char down, float scale) const;
	Int2 GetArrowInput() const;
	int GetInput(char positive, char negative) const;

	static bool IsAzerty();
	static char ScanCodeToVk(char scanCode);
	static char VkToScanCode(char virtualKey);

};

template <class T>
bool Keyboard_<T>::IsDown(char c) const
{
	return reinterpret_cast<const T*>(this)->IsDown_(c);
}

template <class T>
Float2 Keyboard_<T>::GetWasdInput(float scale) const
{
	return GetArrowInput('Q', 'D', 'Z', 'S', scale);
}

template<class T>
inline Float2 Keyboard_<T>::GetArrowInput(char left, char right, char up, char down, float scale) const
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
		IsDown(KeyCodes::ARROW_RIGHT) - IsDown(KeyCodes::ARROW_LEFT),
		IsDown(KeyCodes::ARROW_UP) - IsDown(KeyCodes::ARROW_DOWN) };
}

template <class T>
int Keyboard_<T>::GetInput(char positive, char negative) const
{
	return IsDown(positive) - IsDown(negative);
}
template<class T>
bool Keyboard_<T>::IsAzerty()
{
	const char qValue{ ScanCodeToVk(ScanCode::Q) };

	return qValue == KeyCodes::A;
}
template<class T>
inline char Keyboard_<T>::ScanCodeToVk(char scanCode)
{
	return static_cast<char>(
		MapVirtualKeyA(scanCode, MAPVK_VSC_TO_VK));
}
template<class T>
inline char Keyboard_<T>::VkToScanCode(char virtualKey)
{
	return static_cast<char>(
		MapVirtualKeyA(virtualKey, MAPVK_VK_TO_VSC));
}
}
}
}
