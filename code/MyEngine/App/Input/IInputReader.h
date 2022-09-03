#pragma once

namespace MyEngine
{
	namespace App
	{
		namespace Input
		{
			class IInputReader
			{
			public:
				IInputReader(const IInputReader& other) = delete;
				IInputReader(IInputReader&& other) noexcept = delete;
				IInputReader& operator=(const IInputReader& other) = delete;
				IInputReader& operator=(IInputReader&& other) noexcept = delete;

				IInputReader() = default;
				virtual ~IInputReader() = default;

				virtual bool IsKeyPressed(char character) = 0;
				virtual bool IsKeyReleased(char character) = 0;
				virtual bool IsKeyDown(char character) = 0;
			};
		}
	}
}