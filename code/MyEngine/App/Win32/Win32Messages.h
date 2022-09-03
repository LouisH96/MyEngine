#pragma once

#include "../IOsMessages.h"

namespace MyEngine
{
	namespace App
	{
		namespace Win32
		{
			class Win32Messages final
				: public IOsMessages
			{
			public:
				void HandleMessages(Input::IInputWriter& inputWriter) override;
			};
		}
	}
}
