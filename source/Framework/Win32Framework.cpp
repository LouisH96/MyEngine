#include "Win32Framework.h"

#include <Windows.h>
#include <string>
#include "../Logging/Logger.h"

LRESULT CALLBACK window_proc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);

MyEngine::Framework::Win32Framework::Win32Framework(const std::wstring& title)
{
	Init(title);
}

void MyEngine::Framework::Win32Framework::Init(const std::wstring& title)
{
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
	HWND windowHandle = CreateWindowEx(
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
	ShowWindow(windowHandle, true);

	//Run message loop
	MSG msg{};
	while (GetMessage(&msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
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

		MyEngine::Logging::Logger::Print("paint");
	}
	return 0;
	}
}