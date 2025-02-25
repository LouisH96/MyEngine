#include "pch.h"
// ReSharper disable CppClangTidyPerformanceNoIntToPtr
#include "Window.h"

#include "IExtraWinProc.h"
#include "Screen.h"
#include <Rendering/Canvas.h>

App::Win32::Window::Window(const std::wstring& title, Options options)
	: m_pExtraWinProc{ nullptr }
{
	Init(title, options);
}

App::Win32::Window::Window(const std::wstring& title, IExtraWinProc& extraWinProc, Options options)
	: m_pExtraWinProc{ &extraWinProc }
{
	Init(title, options);
}

App::Win32::Window::Window(const std::wstring& title, const Int2& clientSize,
	Options options)
	: m_ClientSize(clientSize.x, clientSize.y)
	, m_pExtraWinProc{ nullptr }
{
	Init(title, options);
}

App::Win32::Window::Window(const std::wstring& title, const Int2& clientSize,
	IExtraWinProc& extraWinProc, Options options)
	: m_ClientSize(clientSize.x, clientSize.y)
	, m_pExtraWinProc{ &extraWinProc }
{
	Init(title, options);
}

App::Win32::Window::~Window()
{
	Release();
}

void App::Win32::Window::Init(const std::wstring& title, const Options& options)
{
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

	//Size & Pos
	Int2 windowPos, windowSize;
	bool maximizeWindow;
	FindInitPosAndSize(options.ClientSize, windowPos, windowSize, maximizeWindow);

	//Create window
	m_WindowHandle = CreateWindowEx(
		0,                              // Optional window styles.
		className.c_str(),              // Window class
		title.c_str(),    // Window text
		WINDOW_STYLE,            // Window style

		//Position & Size
		windowPos.x, windowPos.y,
		windowSize.x, windowSize.y,

		nullptr,       // Parent window    
		nullptr,       // Menu
		hInstance,  // Instance handle
		nullptr        // Additional application data
	);

	SetWindowLongPtr(m_WindowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	ShowWindow(m_WindowHandle, maximizeWindow ? SW_MAXIMIZE : SW_NORMAL);
	SetCursorFocusMode(options.CursorFocusMode);

	//GLOBALS
	if (Globals::pWindow) Logger::Error("Global window already set");
	if (Globals::pMouse) Logger::Error("A second mouse is made");
	if (Globals::pKeyboard) Logger::Error("A second keyboard is made");
	Globals::pWindow = this;
	Globals::pMouse = &m_Mouse;
	Globals::pKeyboard = &m_Keyboard;

	//HandleMessages();
}

void App::Win32::Window::Release()
{
	//todo: check if and how you could quit the app from here, and not from a quit msg on the queue
	//simply PostMessage with wm_quit would work probably, but normally this shouldn't be closed like this
}

void App::Win32::Window::FindInitPosAndSize(const Int2& desiredCanvasSize, Int2& windowPos, Int2& windowSize, bool& maximized)
{
	//get work area
	HMONITOR monitorHandle = MonitorFromWindow(m_WindowHandle, MONITOR_DEFAULTTONEAREST);
	MONITORINFO monitorInfo = { sizeof(monitorInfo) };
	GetMonitorInfo(monitorHandle, &monitorInfo);
	const Int2 workAreaOffset{ monitorInfo.rcWork.left, monitorInfo.rcWork.top };
	const Int2 workAreaSize{
		monitorInfo.rcWork.right - monitorInfo.rcWork.left,
		monitorInfo.rcWork.bottom - monitorInfo.rcWork.top };

	//calculate new size
	RECT windowRect;
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = desiredCanvasSize.x;
	windowRect.bottom = desiredCanvasSize.y;
	AdjustWindowRect(&windowRect, WINDOW_STYLE, false); //add window-border to client-area
	windowSize = { windowRect.right - windowRect.left, windowRect.bottom - windowRect.top };

	unsigned borderCount{ 0 };
	if (windowSize.x >= workAreaSize.x)
	{
		borderCount++;
		windowSize.x = static_cast<int>(workAreaSize.x );
	}
	if (windowSize.y >= workAreaSize.y)
	{
		borderCount++;
		windowSize.y = static_cast<int>(workAreaSize.y );
	}

	maximized = borderCount == 2;
	if (maximized)
	{
		static constexpr float minimizedScale{ .75f };
		windowSize.x = static_cast<int>(windowSize.x * minimizedScale);
		windowSize.y = static_cast<int>(windowSize.y * minimizedScale);
	}

	windowPos = { (workAreaSize - windowSize) / 2 + workAreaOffset };
	maximized = borderCount == 2;
}

void App::Win32::Window::SetCursorFocusMode(bool cursorFocused)
{
	m_CursorFocusMode = cursorFocused;
	if (m_CursorFocusMode)
		while (ShowCursor(false) >= 0) {}
	else
		while (ShowCursor(true) < 0) {}
}

void App::Win32::Window::HandleMessages()
{
	m_IsResized = false;
	m_Mouse.PreChange();
	m_Keyboard.PreInput();

	//win32-messages
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		DispatchMessage(&msg);

	m_Mouse.PostChange();

	if (m_CursorFocusMode && m_HasFocus)
	{
		tagPOINT pos{ m_ClientSize.x / 2,m_ClientSize.y / 2 };
		m_Mouse.SetPos({ pos.x, pos.y });
		ClientToScreen(m_WindowHandle, &pos);
		SetCursorPos(pos.x, pos.y);
	}
}

LRESULT CALLBACK win32_window_proc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	App::Win32::Window& window = *reinterpret_cast<App::Win32::Window*>(GetWindowLongPtr(windowHandle, GWLP_USERDATA));

	switch (uMsg)
	{
	case WM_DESTROY:
		window.m_IsDestroyed = true;
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		window.m_ClientSize.x = LOWORD(lParam);
		window.m_ClientSize.y = HIWORD(lParam);
		window.m_IsResized = window.m_ClientSize.x > 0 && window.m_ClientSize.y > 0;
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
	using namespace App::Win32;
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