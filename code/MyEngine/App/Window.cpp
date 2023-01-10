#include "pch.h"
// ReSharper disable CppClangTidyPerformanceNoIntToPtr
#include "Window.h"
#include "App/Screen.h"
#include "Gpu/Dx/DxCanvas.h"

MyEngine::App::Window::Window(const std::wstring& title)
{
	Init(title);
}

MyEngine::App::Window::~Window()
{
	Release();
}

void MyEngine::App::Window::Init(const std::wstring& title, int width, int height)
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
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	RegisterClass(&windowClass);

	//Get the entire screen center
	const Screen& screen = Screen::GetInstance();
	const int screenCenterX = screen.GetRect().CenterX();
	const int screenCenterY = screen.GetRect().CenterY();

	//Create window
	// ReSharper disable once CppLocalVariableMayBeConst
	m_WindowHandle = CreateWindowEx(
		0,                              // Optional window styles.
		className.c_str(),              // Window class
		title.c_str(),    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		screenCenterX - (width / 2),
		screenCenterY - (height / 2),
		width, height,

		nullptr,       // Parent window    
		nullptr,       // Menu
		hInstance,  // Instance handle
		nullptr        // Additional application data
	);
	SetWindowLongPtr(m_WindowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	ShowWindow(m_WindowHandle, true);
	m_Size = { width,height };
}

void MyEngine::App::Window::Release()
{
	//todo: check if and how you could quit the app from here, and not from a quit msg on the queue
	//simply PostMessage with wm_quit would work probably, but normally this shouldn't be closed like this
}

void MyEngine::App::Window::HandleMessages()
{
	m_IsResized = false;

	//win32-messages
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		DispatchMessage(&msg);
}

DirectX::XMINT2 MyEngine::App::Window::GetSize_WinApi() const
{
	RECT rect{};
	GetClientRect(m_WindowHandle, &rect);
	return { rect.right - rect.left, rect.bottom - rect.top };
}

LRESULT CALLBACK win32_window_proc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	MyEngine::App::Window& window = *reinterpret_cast<MyEngine::App::Window*>(GetWindowLongPtr(windowHandle, GWLP_USERDATA));

	switch (uMsg)
	{
	case WM_DESTROY:
		window.m_IsDestroyed = true;
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		window.m_Size.x = LOWORD(lParam);
		window.m_Size.y = HIWORD(lParam);
		window.m_IsResized = true;
		break;
	case WM_KEYDOWN:
		window.m_Keyboard.KeyDown(static_cast<char>(wParam));
		break;
	case WM_KEYUP:
		window.m_Keyboard.KeyUp(static_cast<char>(wParam));
		break;
	default:;
	}
	return DefWindowProc(windowHandle, uMsg, wParam, lParam);
}
