#pragma once

namespace MyEngine
{
	namespace App
	{
		class IOsMessages
		{
		public:
			IOsMessages(const IOsMessages& other) = delete;
			IOsMessages(IOsMessages&& other) noexcept = delete;
			IOsMessages& operator=(const IOsMessages& other) = delete;
			IOsMessages& operator=(IOsMessages&& other) noexcept = delete;

			IOsMessages() = default;
			virtual ~IOsMessages() = default;

			virtual void HandleMessages() = 0;
		};
	}
}