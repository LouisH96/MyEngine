#pragma once

namespace MyEngine
{
	class KeyCodes
	{
	public:
		static constexpr char ESCAPE = '\x1b';
		static constexpr char SHIFT_LEFT = '\x10';
		static constexpr char CTRL_LEFT = '\x11';
		static constexpr char ARROW_LEFT = 37;
		static constexpr char ARROW_UP = 38;
		static constexpr char ARROW_RIGHT = 39;
		static constexpr char ARROW_DOWN = 40;

		static constexpr char F = 'F';
	};
}