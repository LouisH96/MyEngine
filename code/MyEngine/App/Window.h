#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <string>
#include <vector>
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
#pragma region C++
		public:
			Window(const Window& other) = delete;
			Window(Window&& other) noexcept = delete;
			Window& operator=(const Window& other) = delete;
			Window& operator=(Window&& other) noexcept = delete;

			Window(const std::wstring& title);
			~Window();
#pragma endregion

#pragma region Object
		public:
			void Init(const std::wstring& title, int width = 1200, int height = 800);
			void Release();
#pragma endregion

#pragma region Messages
		public:
			void HandleMessages();
#pragma endregion

#pragma region Screen
		public:
			bool IsDestroyed() const { return m_IsDestroyed; }
			DirectX::XMINT2 GetSize_WinApi() const;
			DirectX::XMINT2 GetSize() const { return m_Size; };
			bool IsResized() const { return m_IsResized; }
		private:
			bool m_IsDestroyed{ false };
			DirectX::XMINT2 m_Size{};
			bool m_IsResized{ false };
#pragma endregion

#pragma region Input
		public:
			const Input::Keyboard_MsgListener& GetKeyboard() const { return m_Keyboard; }
		private:
			Input::Keyboard_MsgListener m_Keyboard{};

#pragma endregion

#pragma region Win32
		public:
			HWND GetWindowHandle()const { return m_WindowHandle; }
		private:
			HWND m_WindowHandle{};
			friend LRESULT CALLBACK::win32_window_proc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);
#pragma endregion
		};
	}
}
