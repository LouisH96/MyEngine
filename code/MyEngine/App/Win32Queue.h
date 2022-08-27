#pragma once

namespace MyEngine
{
	namespace App
	{
		class Win32Queue
		{
		public:
			Win32Queue(const Win32Queue& other) = delete;
			Win32Queue(Win32Queue&& other) noexcept = delete;
			Win32Queue& operator=(const Win32Queue& other) = delete;
			Win32Queue& operator=(Win32Queue&& other) noexcept = delete;

			Win32Queue() = default;
			~Win32Queue() = default;

			void HandleMessages();

			bool IsActive() const { return m_IsActive; }
			void ResetState() { m_IsActive = true; }

		private:
			bool m_IsActive = true;
		};
	}
}
