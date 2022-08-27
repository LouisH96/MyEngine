#pragma once

namespace MyEngine
{
	namespace App
	{
		class Win32MsgHandler
		{
		public:
			Win32MsgHandler(const Win32MsgHandler& other) = delete;
			Win32MsgHandler(Win32MsgHandler&& other) noexcept = delete;
			Win32MsgHandler& operator=(const Win32MsgHandler& other) = delete;
			Win32MsgHandler& operator=(Win32MsgHandler&& other) noexcept = delete;

			Win32MsgHandler() = default;
			~Win32MsgHandler() = default;

			void HandleMessages();

			bool IsActive() const { return m_IsActive; }
			void ResetState() { m_IsActive = true; }

		private:
			bool m_IsActive = true;
		};
	}
}
