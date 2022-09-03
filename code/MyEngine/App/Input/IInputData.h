#pragma once

namespace MyEngine
{
	namespace App
	{
		namespace Input
		{
			class IInputWriter;
			class IInputReader;

			class IInputData
			{
			public:
				IInputData(const IInputData& other) = delete;
				IInputData(IInputData&& other) noexcept = delete;
				IInputData& operator=(const IInputData& other) = delete;
				IInputData& operator=(IInputData&& other) noexcept = delete;

				IInputData() = default;
				virtual~IInputData() = default;

				virtual IInputWriter* CreateWriter() = 0;
				virtual IInputReader* CreateReader() = 0;
			};
		}
	}
}