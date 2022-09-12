// ReSharper disable CppClangTidyPerformanceNoIntToPtr
#include "Win32Window.h"
#include "../IWindowResizeListener.h"
#include "../Input/InputWriter.h"
#include "../Input/EmptyInputWriter.h"

MyEngine::App::Win32::Win32Window::Win32Window(const std::wstring& title)
	: m_pEmptyInputWriter(new Input::EmptyInputWriter())
{
	m_pInputWriter = m_pEmptyInputWriter;
	Init(title);
}

MyEngine::App::Win32::Win32Window::~Win32Window()
{
	Release();
}

void MyEngine::App::Win32::Win32Window::Init(const std::wstring& title)
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
	m_NewSize = { 0,0 };
}

void MyEngine::App::Win32::Win32Window::Release()
{
	//todo: check if and how you could quit the app from here, and not from a quit msg on the queue
	//simply PostMessage with wm_quit would work probably, but normally this shouldn't be closed like this

	//Input
	delete m_pEmptyInputWriter;
	m_pEmptyInputWriter = nullptr;
}

void MyEngine::App::Win32::Win32Window::Listen(IWindowResizeListener& listener)
{
	m_ResizeListeners.push_back(&listener);
}

void MyEngine::App::Win32::Win32Window::HandleMessages()
{
	//win32-messages
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//resize event
	if (m_NewSize.x != 0)
	{
		for (auto* pListener : m_ResizeListeners)
			pListener->OnWindowResized(m_NewSize);
		m_NewSize.x = 0;
		m_NewSize.y = 0;
	}
}

DirectX::XMINT2 MyEngine::App::Win32::Win32Window::GetSize() const
{
	RECT rect{};
	GetClientRect(m_WindowHandle, &rect);
	return { rect.right - rect.left, rect.bottom - rect.top };
}

void MyEngine::App::Win32::Win32Window::SetInputWriter(Input::IInputWriter& writer)
{
	if (m_pInputWriter == m_pEmptyInputWriter)
	{
		delete m_pEmptyInputWriter;
		m_pEmptyInputWriter = nullptr;
	}
	m_pInputWriter = &writer;
}

#include "../../Logging/Logger.h"
LRESULT CALLBACK win32_window_proc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	using namespace MyEngine::App::Win32;
	Win32Window& window = *reinterpret_cast<Win32Window*>(GetWindowLongPtr(windowHandle, GWLP_USERDATA));

	switch (uMsg)
	{
	case WM_DESTROY:
	{
		window.m_IsDestroyed = true;
		PostQuitMessage(0);
	}
	case WM_SIZE:
	{
		window.m_NewSize.x = LOWORD(lParam);
		window.m_NewSize.y = HIWORD(lParam);
	}
	case WM_KEYDOWN:
		if (!(lParam & 1 << 30))
			window.m_pInputWriter->OnKeyPressed(static_cast<char>(wParam));
		break;
	case WM_KEYUP:
		window.m_pInputWriter->OnKeyReleased(static_cast<char>(wParam));
		break;
	default:
		return DefWindowProc(windowHandle, uMsg, wParam, lParam);
	}
	return 0;
}
