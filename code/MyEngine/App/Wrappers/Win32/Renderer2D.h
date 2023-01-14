#pragma once
#include <Windows.h>
#include <gdiplus.h>

namespace MyEngine
{
	namespace App
	{
		namespace Wrappers
		{
			namespace Win32
			{
				class Window;
			}
		}
	}
}

namespace MyEngine
{
	namespace App
	{
		namespace Wrappers
		{
			namespace Win32
			{
				class Renderer2D
				{
				public:
					Renderer2D(MyEngine::App::Wrappers::Win32::Window& window);
					~Renderer2D();

					void Present() const;
					Gdiplus::Graphics& GetGraphics() const { return *m_pGraphics; }

				private:
					MyEngine::App::Wrappers::Win32::Window& m_Window;
					ULONG_PTR m_GdiToken{};
					HDC m_WindowContext{};
					HDC m_BackBufferContext{};
					HBITMAP m_BackBufferBitmap{};
					HANDLE m_Handle{}; //idk

					Gdiplus::Graphics* m_pGraphics{};
				};
			}
		}
	}
}
