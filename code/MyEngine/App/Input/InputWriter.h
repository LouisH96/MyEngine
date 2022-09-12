#pragma once
#include "IInputWriter.h"

namespace MyEngine
{
	namespace App
	{
		namespace Input
		{
			class InputData;

			class InputWriter final
				: public IInputWriter
			{
			public:
				explicit InputWriter(InputData& inputData);

				void OnKeyPressed(char character) const override;
				void OnKeyReleased(char character) const override;

			private:
				InputData& m_Data;
			};
		}
	}
}
