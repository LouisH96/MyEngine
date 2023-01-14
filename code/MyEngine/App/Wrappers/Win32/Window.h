#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include <string>
#include "Keyboard_MsgListener.h"

LRESULT CALLBACK win32_window_proc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK win32_window_proc_extra(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace MyEngine
{
	namespace App
	{
		namespace Wrappers
		{
			namespace Win32
			{
				class IExtraWinProc;

				class Window
				{
				public:
					struct Options
					{
						bool FullRedrawOnScale{ false };
						HBRUSH BackgroundBrush{ nullptr };
					};

				public:
					Window(const Window& other) = delete;
					Window(Window&& other) noexcept = delete;
					Window& operator=(const Window& other) = delete;
					Window& operator=(Window&& other) noexcept = delete;

					explicit Window(const std::wstring& title, Options options = {});
					explicit Window(const std::wstring& title, IExtraWinProc& extraWinProc, Options options = {});
					~Window();

					//window
					bool IsDestroyed() const { return m_IsDestroyed; }
					bool IsResized() const { return m_IsResized; }
					DirectX::XMINT2 GetClientSize() const { return m_ClientSize; };
					int GetClientWidth() const { return m_ClientSize.x; }
					int GetClientHeight() const { return m_ClientSize.y; }
					DirectX::XMINT2 AskClientSize_WinApi() const;
					HWND GetWindowHandle()const { return m_WindowHandle; }

					//messages
					void HandleMessages();

					//input
					const Keyboard_MsgListener& GetKeyboard() const { return m_Keyboard; }

				private:
					void Init(const std::wstring& title, const Options& options, int clientWidth = 1200, int clientHeight = 800);
					void Release();

					Keyboard_MsgListener m_Keyboard{};
					DirectX::XMINT2 m_ClientSize{};
					HWND m_WindowHandle{};
					IExtraWinProc* m_pExtraWinProc{};
					bool m_IsDestroyed{ false };
					bool m_IsResized{ false };

					friend LRESULT CALLBACK::win32_window_proc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);
					friend LRESULT CALLBACK::win32_window_proc_extra(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);
				};
			}
		}
	}
}
