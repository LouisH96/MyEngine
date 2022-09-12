#pragma once
#include "IInputWriter.h"

namespace MyEngine
{
	namespace App
	{
		namespace Input
		{
			class EmptyInputWriter final
				: public IInputWriter
			{
			public:
				void OnKeyPressed(char) const override {};
				void OnKeyReleased(char) const override {}
			};
		}
	}
}

