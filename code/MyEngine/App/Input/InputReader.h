#pragma once
#include "IInputReader.h"

namespace MyEngine
{
	namespace App
	{
		namespace Input
		{
			class InputData;

			class InputReader final
				: public IInputReader
			{
			public:
				explicit InputReader(InputData& data);

				bool IsKeyPressed(char character) override;
				bool IsKeyReleased(char character) override;
				bool IsKeyDown(char character) override;

			private:
				InputData& m_Data;
			};
		}
	}
}
