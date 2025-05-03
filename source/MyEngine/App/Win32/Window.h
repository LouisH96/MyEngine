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
					Float3 BackgroundColor{ Float3::Color(100, 149, 237) };
					bool CursorFocusMode{ false };
					Int2 ClientSize{ 1200,800 };
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
				HWND GetWindowHandle()const { return m_WindowHandle; }
				void SetCursorFocusMode(bool cursorFocused);

				//Canvas (should be moved to Canvas class?)
				Int2 GetClientSize() const { return m_ClientSize; }
				int GetClientWidth() const { return m_ClientSize.x; }
				int GetClientHeight() const { return m_ClientSize.y; }
				Float2 GetClientSizeF() const { return m_ClientSize; }
				float GetClientWidthF() const {	return static_cast<float>(m_ClientSize.x); }
				float GetClientHeightF() const { return static_cast<float>(m_ClientSize.y); }
				float GetAspectRatio() const { return static_cast<float>(m_ClientSize.x) / m_ClientSize.y; }

				//messages
				void HandleMessages();

				//input
				const Keyboard& GetKeyboard() const { return m_Keyboard; }
				const Mouse& GetMouse() const { return m_Mouse; }

			private:
				static constexpr int INIT_POS{ 500 };
				static constexpr unsigned INIT_SIZE{ 500 };
				static constexpr DWORD WINDOW_STYLE{ WS_OVERLAPPEDWINDOW };

				void Init(const std::wstring& title, const Options& options);
				void Release();

				void FindInitPosAndSize(const Int2& desiredCanvasSize, Int2& windowPos, Int2& windowSize, bool& maximized);

				Keyboard m_Keyboard{};
				Mouse m_Mouse{};
				Int2 m_ClientSize{ 1200,800 };
				HWND m_WindowHandle{};
				IExtraWinProc* m_pExtraWinProc{};
				bool m_IsDestroyed{ false };
				bool m_IsResized{ false };
				bool m_HasFocus{ true };
				bool m_CursorFocusMode{ false };

				static uint8_t GetVirtualKey(const WPARAM& wParam, const LPARAM& lParam);

				friend LRESULT CALLBACK::win32_window_proc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);
				friend LRESULT CALLBACK::win32_window_proc_extra(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);
			};
		}
	}
}
