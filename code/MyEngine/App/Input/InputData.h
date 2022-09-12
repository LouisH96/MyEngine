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

				void Update() override;

			private:
				static constexpr size_t NR_OF_KEYS = 256;
				enum KeyStatesType
				{
					Down, Pressed, Released, Count
				};
				
				bool m_KeyStates[KeyStatesType::Count][NR_OF_KEYS]{{false}};

				friend class InputWriter;
				friend class InputReader;
			};
		}
	}
}

