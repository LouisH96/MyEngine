#include "pch.h"
// ReSharper disable CppClangTidyPerformanceNoIntToPtr
#include "Window.h"

#include "IExtraWinProc.h"
#include "Screen.h"
#include <Rendering/Canvas.h>
#include "Math/Int2.h"

MyEngine::App::Win32::Window::Window(const std::wstring& title, Options options)
	: m_pExtraWinProc{ nullptr }
{
	Init(title, options);
}

MyEngine::App::Win32::Window::Window(const std::wstring& title, IExtraWinProc& extraWinProc, Options options)
	: m_pExtraWinProc{ &extraWinProc }
{
	Init(title, options);
}

MyEngine::App::Win32::Window::Window(const std::wstring& title, const Math::Int2& clientSize,
	Options options)
	: m_ClientSize(clientSize.x, clientSize.y)
	, m_pExtraWinProc{ nullptr }
{
	Init(title, options);
}

MyEngine::App::Win32::Window::Window(const std::wstring& title, const Math::Int2& clientSize,
	IExtraWinProc& extraWinProc, Options options)
	: m_ClientSize(clientSize.x, clientSize.y)
	, m_pExtraWinProc{ &extraWinProc }
{
	Init(title, options);
}

MyEngine::App::Win32::Window::~Window()
{
	Release();
}

void MyEngine::App::Win32::Window::Init(const std::wstring& title, const Options& options)
{
	//options
	m_CursorFpsMode = options.CursorFpsMode;
	if (options.ClientSize.x > 0) m_ClientSize.x = options.ClientSize.x;
	if (options.ClientSize.y > 0) m_ClientSize.y = options.ClientSize.y;

	//Register window class
	const std::wstring className = L"MyWindowClass";
	// ReSharper disable once CppLocalVariableMayBeConst
	HINSTANCE hInstance = GetModuleHandle(nullptr);

	WNDCLASS windowClass{};
	windowClass.lpfnWndProc = m_pExtraWinProc ? win32_window_proc_extra : win32_window_proc;
	windowClass.lpszClassName = className.c_str();
	windowClass.hInstance = hInstance;
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.style = CS_BYTEALIGNCLIENT;
	if (options.FullRedrawOnScale) windowClass.style |= CS_HREDRAW | CS_VREDRAW;
	windowClass.hbrBackground = options.BackgroundBrush;
	RegisterClass(&windowClass);

	//Get the entire screen center
	const Screen& screen = Screen::GetInstance();
	const Math::RectInt& screenRect = screen.GetRect();

	//Create window
	constexpr DWORD windowStyle = WS_OVERLAPPEDWINDOW;
	const RECT clientRect
	{
		 (screenRect.GetWidth() - m_ClientSize.x) / 2, //left
		 (screenRect.GetHeight() - m_ClientSize.y) / 2, //top
		 (screenRect.GetWidth() - m_ClientSize.x) / 2 + m_ClientSize.x, //right
		 (screenRect.GetHeight() - m_ClientSize.y) / 2 + m_ClientSize.y //bottom
	};
	RECT rect = clientRect;
	AdjustWindowRect(&rect, windowStyle, false); //from desired client-rect to window-rect
	const RECT windowRect = rect;

	m_WindowHandle = CreateWindowEx(
		0,                              // Optional window styles.
		className.c_str(),              // Window class
		title.c_str(),    // Window text
		windowStyle,            // Window style

		// Size and position
		windowRect.left, windowRect.top,
		windowRect.right - windowRect.left, //windowRect-width
		windowRect.bottom - windowRect.top, //windowRect-height

		nullptr,       // Parent window    
		nullptr,       // Menu
		hInstance,  // Instance handle
		nullptr        // Additional application data
	);
	SetWindowLongPtr(m_WindowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	ShowWindow(m_WindowHandle, true);
	ShowCursor(!m_CursorFpsMode);
}

void MyEngine::App::Win32::Window::Release()
{
	//todo: check if and how you could quit the app from here, and not from a quit msg on the queue
	//simply PostMessage with wm_quit would work probably, but normally this shouldn't be closed like this
}

void MyEngine::App::Win32::Window::HandleMessages()
{
	m_IsResized = false;
	m_Mouse.PreChange();

	//win32-messages
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		DispatchMessage(&msg);

	m_Mouse.PostChange();

	if (m_CursorFpsMode && m_HasFocus)
	{
		tagPOINT pos{ m_ClientSize.x / 2,m_ClientSize.y / 2 };
		m_Mouse.SetPos({ pos.x, pos.y });
		ClientToScreen(m_WindowHandle, &pos);
		SetCursorPos(pos.x, pos.y);
	}
}

DirectX::XMINT2 MyEngine::App::Win32::Window::AskClientSize_WinApi() const
{
	RECT rect{};
	GetClientRect(m_WindowHandle, &rect);
	return { rect.right - rect.left, rect.bottom - rect.top };
}

LRESULT CALLBACK win32_window_proc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	MyEngine::App::Win32::Window& window = *reinterpret_cast<MyEngine::App::Win32::Window*>(GetWindowLongPtr(windowHandle, GWLP_USERDATA));

	switch (uMsg)
	{
	case WM_DESTROY:
		window.m_IsDestroyed = true;
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED) break;
		window.m_ClientSize.x = LOWORD(lParam);
		window.m_ClientSize.y = HIWORD(lParam);
		window.m_IsResized = true;
		break;
	case WM_KEYDOWN:
		window.m_Keyboard.KeyDown(static_cast<char>(wParam));
		break;
	case WM_KEYUP:
		window.m_Keyboard.KeyUp(static_cast<char>(wParam));
		break;
	case WM_MOUSEMOVE:
		window.m_Mouse.OnMove(lParam);
		break;
	case WM_LBUTTONDOWN:
		SetCapture(window.GetWindowHandle());
		window.m_Mouse.OnLeftBtnPressed();
		break;
	case WM_LBUTTONUP:
		ReleaseCapture();
		window.m_Mouse.OnLeftBtnReleased();
		break;
	case WM_MBUTTONDOWN:
		SetCapture(window.GetWindowHandle());
		window.m_Mouse.OnMiddleBtnPressed();
		break;
	case WM_MBUTTONUP:
		ReleaseCapture();
		window.m_Mouse.OnMiddleBtnReleased();
		break;
	case WM_RBUTTONDOWN:
		SetCapture(window.GetWindowHandle());
		window.m_Mouse.OnRightBtnPressed();
		break;
	case WM_RBUTTONUP:
		ReleaseCapture();
		window.m_Mouse.OnRightBtnReleased();
		break;
	case WM_MOUSEWHEEL:
		window.m_Mouse.OnScroll(static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / 120.f);
		break;
	case WM_SETFOCUS:
		window.m_HasFocus = true;
		break;
	case WM_KILLFOCUS:
		window.m_HasFocus = false;
		break;
	default:;
	}
	return DefWindowProc(windowHandle, uMsg, wParam, lParam);
}

LRESULT CALLBACK win32_window_proc_extra(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	using namespace MyEngine::App::Win32;
	const Window& window = *reinterpret_cast<Window*>(GetWindowLongPtr(windowHandle, GWLP_USERDATA));

	switch (uMsg)
	{
	case WM_DESTROY:
		break;
	case WM_SIZE:
		window.m_pExtraWinProc->OnResize(window.GetClientSize());
		break;
	case WM_KEYDOWN:
		window.m_pExtraWinProc->OnKeyDown(static_cast<char>(wParam));
		break;
	case WM_KEYUP:
		window.m_pExtraWinProc->OnKeyUp(static_cast<char>(wParam));
		break;
	case WM_PAINT:
		window.m_pExtraWinProc->OnPaint();
		break;
	default:;
	}
	return win32_window_proc(windowHandle, uMsg, wParam, lParam);
}