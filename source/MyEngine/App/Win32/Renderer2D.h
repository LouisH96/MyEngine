#pragma once
#include <Windows.h>

namespace MyEngine
{
	namespace App
	{
		namespace Win32
		{
			class Window;
		}
	}
}

namespace MyEngine
{
	namespace App
	{
		namespace Win32
		{
			class Renderer2D
			{
			public:
				Renderer2D(MyEngine::App::Win32::Window& window);
				~Renderer2D();

				void Present() const;
				HDC GetBackBufferContext() const { return m_BackBufferContext; }

			private:
				Window& m_Window;
				HDC m_WindowContext{};
				HDC m_BackBufferContext{};
				HBITMAP m_BackBufferBitmap{};
				HANDLE m_Handle{}; //idk
			};
		}
	}
}
