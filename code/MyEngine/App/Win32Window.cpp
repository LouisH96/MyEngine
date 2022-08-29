// ReSharper disable CppClangTidyPerformanceNoIntToPtr
#include "Win32Window.h"

MyEngine::App::Win32Window::Win32Window(const std::wstring& title)
{
	Init(title);
}

MyEngine::App::Win32Window::~Win32Window()
{
	Release();
}

void MyEngine::App::Win32Window::Init(const std::wstring& title)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	//Register window class
	const std::wstring className = L"MyWindowClass";
	// ReSharper disable once CppLocalVariableMayBeConst
	HINSTANCE hInstance = GetModuleHandle(nullptr);

	WNDCLASS windowClass{};
	windowClass.lpfnWndProc = win32_window_proc;
	windowClass.lpszClassName = className.c_str();
	windowClass.hInstance = hInstance;
	RegisterClass(&windowClass);

	//Create window
	// ReSharper disable once CppLocalVariableMayBeConst
	m_WindowHandle = CreateWindowEx(
		0,                              // Optional window styles.
		className.c_str(),              // Window class
		title.c_str(),    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		nullptr,       // Parent window    
		nullptr,       // Menu
		hInstance,  // Instance handle
		nullptr        // Additional application data
	);
	SetWindowLongPtr(m_WindowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	ShowWindow(m_WindowHandle, true);
}

void MyEngine::App::Win32Window::Release()
{
	//todo: check if and how you could quit the app from here, and not from a quit msg on the queue
	//simply PostMessage with wm_quit would work probably, but normally this shouldn't be closed like this
}

LRESULT CALLBACK win32_window_proc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	using namespace MyEngine::App;

	switch (uMsg)
	{
	case WM_DESTROY:
		{
		Win32Window& window = *reinterpret_cast<Win32Window*>(GetWindowLongPtr(windowHandle, GWLP_USERDATA));
		window.m_IsDestroyed = true;
		PostQuitMessage(0);
		return 0;
		}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		// ReSharper disable once CppLocalVariableMayBeConst
		HDC hdc = BeginPaint(windowHandle, &ps);

		// All painting occurs here, between BeginPaint and EndPaint.
		FillRect(hdc, &ps.rcPaint, reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1));  // NOLINT(performance-no-int-to-ptr)

		EndPaint(windowHandle, &ps);
	}
	return 0;

	default:
		return DefWindowProc(windowHandle, uMsg, wParam, lParam);
	}
}