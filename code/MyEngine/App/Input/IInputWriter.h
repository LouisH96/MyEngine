#pragma once

namespace MyEngine
{
	namespace App
	{
		namespace Input
		{
			class IInputWriter
			{
			public:
				IInputWriter(const IInputWriter& other) = delete;
				IInputWriter(IInputWriter&& other) noexcept = delete;
				IInputWriter& operator=(const IInputWriter& other) = delete;
				IInputWriter& operator=(IInputWriter&& other) noexcept = delete;

				IInputWriter() = default;
				virtual ~IInputWriter() = default;

				virtual void OnKeyPressed(char character) const = 0;
				virtual void OnKeyReleased(char character) const = 0;
			};
		}
	}
}