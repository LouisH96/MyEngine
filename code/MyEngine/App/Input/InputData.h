#pragma once
#include "IInputData.h"

namespace MyEngine
{
	namespace App
	{
		namespace Input
		{
			class InputData final
				: public IInputData
			{
			public:
				IInputWriter* CreateWriter() override;
				IInputReader* CreateReader() override;
			};
		}
	}
}

