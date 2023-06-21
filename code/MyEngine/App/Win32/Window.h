#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include <string>
#include "Keyboard_MsgListener.h"
#include "Mouse.h"

LRESULT CALLBACK win32_window_proc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK win32_window_proc_extra(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace MyEngine
{
	namespace App
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
					bool CursorFocusMode{ false };
					Int2 ClientSize{ -1,-1 };
				};

			public:
				using Keyboard = Keyboard_MsgListener;

				Window(const Window& other) = delete;
				Window(Window&& other) noexcept = delete;
				Window& operator=(const Window& other) = delete;
				Window& operator=(Window&& other) noexcept = delete;

				explicit Window(const std::wstring& title, Options options = {});
				explicit Window(const std::wstring& title, IExtraWinProc& extraWinProc, Options options = {});
				explicit Window(const std::wstring& title, const Int2& clientSize, Options options = {});
				explicit Window(const std::wstring& title, const Int2& clientSize, IExtraWinProc& extraWinProc, Options options = {});
				~Window();

				//window
				bool IsDestroyed() const { return m_IsDestroyed; }
				void SetIsDestroyed() { m_IsDestroyed = true; }
				bool IsResized() const { return m_IsResized; }
				Int2 GetClientSize() const { return m_ClientSize; };
				int GetClientWidth() const { return m_ClientSize.x; }
				int GetClientHeight() const { return m_ClientSize.y; }
				DirectX::XMINT2 AskClientSize_WinApi() const;
				HWND GetWindowHandle()const { return m_WindowHandle; }
				float GetAspectRatio() const { return static_cast<float>(m_ClientSize.x) / m_ClientSize.y; }
				void SetCursorFocusMode(bool cursorFocused);

				//messages
				void HandleMessages();

				//input
				const Keyboard& GetKeyboard() const { return m_Keyboard; }
				const Mouse& GetMouse() const { return m_Mouse; }

			private:
				void Init(const std::wstring& title, const Options& options);
				void Release();

				Keyboard m_Keyboard{};
				Mouse m_Mouse{};
				Int2 m_ClientSize{ 1200,800 };
				HWND m_WindowHandle{};
				IExtraWinProc* m_pExtraWinProc{};
				bool m_IsDestroyed{ false };
				bool m_IsResized{ false };
				bool m_HasFocus{ true };
				bool m_CursorFocusMode{ false };

				friend LRESULT CALLBACK::win32_window_proc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);
				friend LRESULT CALLBACK::win32_window_proc_extra(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);
			};
		}
	}
}
