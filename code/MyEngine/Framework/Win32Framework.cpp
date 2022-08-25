#include "Win32Framework.h"

#include <string>

LRESULT CALLBACK window_proc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);

MyEngine::Framework::Win32Framework::Win32Framework(const std::wstring& title)
{
	Init(title);
}

bool MyEngine::Framework::Win32Framework::Loop()
{
	bool continueAfter = true;
	//Run message loop
	MSG msg{};
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
			continueAfter = false;
	}
	return continueAfter;
}

void MyEngine::Framework::Win32Framework::Init(const std::wstring& title)
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
	windowClass.lpfnWndProc = window_proc;
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
	ShowWindow(m_WindowHandle, true);
}

LRESULT CALLBACK window_proc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	default:
		return DefWindowProc(windowHandle, uMsg, wParam, lParam);
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

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
	}
}