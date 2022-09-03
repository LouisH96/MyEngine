#pragma once
#include "IInputWriter.h"

namespace MyEngine
{
	namespace App
	{
		namespace Input
		{
			class InputWriter final
				: public IInputWriter
			{
			public:
				void OnKeyPressed(char character) override;
				void OnKeReleased(char character) override;
			};
		}
	}
}
