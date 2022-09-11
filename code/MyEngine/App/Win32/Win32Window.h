#pragma once
#include <string>

#include "../IWindow.h"

#include <Windows.h>
#include <string>
#include <vector>

LRESULT CALLBACK win32_window_proc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace MyEngine
{
	namespace App
	{
		namespace Win32
		{
			class Win32Window final :
				public IWindow
			{
#pragma region C++
			public:
				Win32Window(const Win32Window& other) = delete;
				Win32Window(Win32Window&& other) noexcept = delete;
				Win32Window& operator=(const Win32Window& other) = delete;
				Win32Window& operator=(Win32Window&& other) noexcept = delete;

				Win32Window(const std::wstring& title);
				~Win32Window() override;
#pragma endregion

#pragma region Object
			public:
				void Init(const std::wstring& title) override;
				void Release() override;
#pragma endregion

#pragma region Messages
			public:
				void Listen(IWindowResizeListener& listener) override;
				void HandleMessages() override;
			private:
				std::vector<IWindowResizeListener*> m_ResizeListeners{};
#pragma endregion

#pragma region Screen
			public:
				bool IsDestroyed() const override { return m_IsDestroyed; }
				DirectX::XMINT2 GetSize() const override;
			private:
				bool m_IsDestroyed{ false };
				DirectX::XMINT2 m_NewSize{};
#pragma endregion

#pragma region Win32
			public:
				HWND GetWindowHandle()const { return m_WindowHandle; }
			private:
				HWND m_WindowHandle{};
				friend LRESULT CALLBACK::win32_window_proc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);
#pragma endregion

#pragma region Input
			public:
				void SetInputWriter(Input::IInputWriter& writer) override;
			private:
				Input::IInputWriter* m_pInputWriter{};
#pragma endregion
			};
		}
	}
}
