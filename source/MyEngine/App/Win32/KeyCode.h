#pragma once
#include <cstdint>

namespace MyEngine
{
/*
	These are the scancodes.
	The codes of the 'physical' keys on the keyboard.
*/

using KeyCodeType = uint16_t;
enum KeyCode : KeyCodeType
{
	Escape = 0x01,
	T1 = 0x02,
	T2 = 0x03,
	T3 = 0x04,
	T4 = 0x05,
	T5 = 0x06,
	T6 = 0x07,
	T7 = 0x08,
	T8 = 0x09,
	T9 = 0x0A,
	T0 = 0x0B,
	Minus = 0x0C,
	Equals = 0x0D,
	Backspace = 0x0E,
	Tab = 0x0F,

	Q = 0x10,
	W = 0x11,
	E = 0x12,
	R = 0x13,
	T = 0x14,
	Y = 0x15,
	U = 0x16,
	I = 0x17,
	O = 0x18,
	P = 0x19,

	BracketLeft = 0x1A,
	BracketRight = 0x1B,
	Enter = 0x1C,
	ControlLeft = 0x1D,

	A = 0x1E,
	S = 0x1F,
	D = 0x20,
	F = 0x21,
	G = 0x22,
	H = 0x23,
	J = 0x24,
	K = 0x25,
	L = 0x26,

	Semicolon = 0x27,
	Apostrophe = 0x28,
	Grave = 0x29,
	ShiftLeft = 0x2A,
	Backslash = 0x2B,

	Z = 0x2C,
	X = 0x2D,
	C = 0x2E,
	V = 0x2F,
	B = 0x30,
	N = 0x31,
	M = 0x32,

	Comma = 0x33,
	Preiod = 0x34,
	Slash = 0x35,
	ShiftRight = 0x36,
	NumpadMultiply = 0x37,
	AltLeft = 0x38,
	Space = 0x39,
	CapsLock = 0x3A,

	F1 = 0x3B,
	F2 = 0x3C,
	F3 = 0x3D,
	F4 = 0x3E,
	F5 = 0x3F,
	F6 = 0x40,
	F7 = 0x41,
	F8 = 0x42,
	F9 = 0x43,
	F10 = 0x44,
	NumLock = 0x45,
	ScrollLock = 0x46,
	Numpad7 = 0x47,
	Numpad8 = 0x48,
	Numpad9 = 0x49,
	NumpadMinus = 0x4A,
	Numpad4 = 0x4B,
	Numpad5 = 0x4C,
	Numpad6 = 0x4D,
	NumpadPlus = 0x4E,
	Numpad1 = 0x4F,
	Numpad2 = 0x50,
	Numpad3 = 0x51,
	Numpad0 = 0x52,
	NumpadPeriod = 0x53,
	AltPrintScreen = 0x54, /* Alt + print screen. MapVirtualKeyEx( VK_SNAPSHOT, MAPVK_VK_TO_VSC_EX, 0 ) returns scancode 0x54. */
	BracketAngle = 0x56, /* Key between the left shift and Z. */
	F11 = 0x57,
	F12 = 0x58,
	Oem1 = 0x5a, /* VK_OEM_WSCTRL */
	Oem2 = 0x5b, /* VK_OEM_FINISH */
	Oem3 = 0x5c, /* VK_OEM_JUMP */
	EraseEOF = 0x5d,
	Oem4 = 0x5e, /* VK_OEM_BACKTAB */
	Oem5 = 0x5f, /* VK_OEM_AUTO */
	Zoom = 0x62,
	Help = 0x63,
	F13 = 0x64,
	F14 = 0x65,
	F15 = 0x66,
	F16 = 0x67,
	F17 = 0x68,
	F18 = 0x69,
	F19 = 0x6a,
	F20 = 0x6b,
	F21 = 0x6c,
	F22 = 0x6d,
	F23 = 0x6e,
	Oem_6 = 0x6f, /* VK_OEM_PA3 */
	Katakana = 0x70,
	Oem_7 = 0x71, /* VK_OEM_RESET */
	F24 = 0x76,
	Sbcschar = 0x77,
	Convert = 0x79,
	Nonconvert = 0x7B, /* VK_OEM_PA1 */

	MediaPrevious = 0xE010,
	MediaNext = 0xE019,
	NumpadEnter = 0xE01C,
	ControlRight = 0xE01D,
	VolumeMute = 0xE020,
	LaunchApp2 = 0xE021,
	MediaPlay = 0xE022,
	MediaStop = 0xE024,
	VolumeDown = 0xE02E,
	VolumeUp = 0xE030,
	BrowserHome = 0xE032,
	NumpadDivide = 0xE035,
	PrintScreen = 0xE037,
	/*
	sc_printScreen:
	- make: 0xE02A 0xE037
	- break: 0xE0B7 0xE0AA
	- MapVirtualKeyEx( VK_SNAPSHOT, MAPVK_VK_TO_VSC_EX, 0 ) returns scancode 0x54;
	- There is no VK_KEYDOWN with VK_SNAPSHOT.
	*/
	AltRight = 0xE038,
	Cancel = 0xE046, /* CTRL + Pause */
	Home = 0xE047,
	ArrowUp = 0xE048,
	PageUp = 0xE049,
	ArrowLeft = 0xE04B,
	ArrowRight = 0xE04D,
	End = 0xE04F,
	ArrowDown = 0xE050,
	PageDown = 0xE051,
	Insert = 0xE052,
	Delete = 0xE053,
	MetaLeft = 0xE05B,
	MetaRight = 0xE05C,
	Application = 0xE05D,
	Power = 0xE05E,
	Sleep = 0xE05F,
	Wake = 0xE063,
	BrowserSearch = 0xE065,
	BrowserFavorites = 0xE066,
	BrowserRefresh = 0xE067,
	BrowserStop = 0xE068,
	BrowserForward = 0xE069,
	BrowserBack = 0xE06A,
	LaunchApp1 = 0xE06B,
	LaunchEmail = 0xE06C,
	LaunchMedia = 0xE06D,

	//Pause = 0xE11D45,
};
}