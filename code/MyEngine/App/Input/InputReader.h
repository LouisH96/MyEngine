#pragma once
#include "IInputReader.h"

namespace MyEngine
{
	namespace App
	{
		namespace Input
		{
			class InputReader final
				: public IInputReader
			{
			public:
				bool IsKeyPressed(char character) override;
				bool IsKeyReleased(char character) override;
				bool IsKeyDown(char character) override;
			};
		}
	}
}
