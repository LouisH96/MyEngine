#pragma once
#include <string>
#include <Windows.h>

namespace MyEngine
{
	namespace Framework
	{
		class Win32Framework
		{
		public:
			Win32Framework(const std::wstring& title);

			bool Loop();

			HWND GetWindowHandle()const { return m_WindowHandle; }

		private:
			void Init(const std::wstring& title);

			HWND m_WindowHandle{};
		};
	}
}
