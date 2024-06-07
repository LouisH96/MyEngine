#pragma once


namespace MyEngine
{
	class KeyCodes
	{
	public:
		static constexpr char SPACE = 0x20;
		static constexpr char ESCAPE = '\x1b';
		static constexpr char SHIFT_LEFT = '\x10';
		static constexpr char CTRL_LEFT = '\x11';
		static constexpr char ARROW_LEFT = 37;
		static constexpr char ARROW_UP = 38;
		static constexpr char ARROW_RIGHT = 39;
		static constexpr char ARROW_DOWN = 40;

		static constexpr char A = 'A';
		static constexpr char B = 'B';
		static constexpr char C = 'C';
		static constexpr char D = 'D';
		static constexpr char E = 'E';
		static constexpr char F = 'F';
		static constexpr char R = 'R';
		static constexpr char Q = 'Q';
	};
}