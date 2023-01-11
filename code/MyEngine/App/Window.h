#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include <string>
#include "Input/Keyboard_MsgListener.h"

namespace MyEngine
{
	namespace App
	{
		class IWindowResizeListener;
	}
}

LRESULT CALLBACK win32_window_proc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace MyEngine
{
	namespace App
	{
		class Window
		{
		public:
			Window(const Window& other) = delete;
			Window(Window&& other) noexcept = delete;
			Window& operator=(const Window& other) = delete;
			Window& operator=(Window&& other) noexcept = delete;

			explicit Window(const std::wstring& title);
			~Window();

			//window
			bool IsDestroyed() const { return m_IsDestroyed; }
			bool IsResized() const { return m_IsResized; }
			DirectX::XMINT2 GetSize() const { return m_Size; };
			DirectX::XMINT2 GetSize_WinApi() const;
			HWND GetWindowHandle()const { return m_WindowHandle; }

			//messages
			void HandleMessages();

			//input
			const Input::Keyboard_MsgListener& GetKeyboard() const { return m_Keyboard; }

		private:
			void Init(const std::wstring& title, int width = 1200, int height = 800);
			void Release();

			Input::Keyboard_MsgListener m_Keyboard{};
			DirectX::XMINT2 m_Size{};
			HWND m_WindowHandle{};
			bool m_IsDestroyed{ false };
			bool m_IsResized{ false };

			friend LRESULT CALLBACK::win32_window_proc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);
		};
	}
}
