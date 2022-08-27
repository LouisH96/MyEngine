#pragma once
#include <string>

#include "IWindow.h"

#include <Windows.h>
#include <string>

namespace MyEngine
{
	namespace App
	{
		class Win32Window final :
			public IWindow
		{
			public:
			Win32Window(const Win32Window& other) = delete;
			Win32Window(Win32Window&& other) noexcept = delete;
			Win32Window& operator=(const Win32Window& other) = delete;
			Win32Window& operator=(Win32Window&& other) noexcept = delete;

			Win32Window(const std::wstring& title);
			~Win32Window() override;

			void Init(const std::wstring& title) override;
			void Release() override;

			HWND GetWindowHandle()const { return m_WindowHandle; }

		private:
			HWND m_WindowHandle{};
		};
	}
}
