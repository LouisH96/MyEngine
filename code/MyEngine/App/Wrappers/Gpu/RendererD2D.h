#pragma once
#include <d2d1helper.h>

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
			namespace Gpu
			{
				class RendererD2D
				{
				public:
					RendererD2D(Win32::Window& window, D2D1::ColorF clearColor = D2D1::ColorF::White);
					~RendererD2D();

					ID2D1SolidColorBrush* CreateBrush(D2D1::ColorF color) const;

					void Clear() const;
					void Clear(D2D1::ColorF color) const;

					ID2D1HwndRenderTarget& GetRenderTarget() const { return *m_pRenderTarget; }
					const D2D1::ColorF& GetClearColor() const { return m_ClearColor; }
					void SetClearColor(const D2D1::ColorF& clearColor);

				private:
					ID2D1Factory* m_pFactory{};
					ID2D1HwndRenderTarget* m_pRenderTarget{};
					D2D1::ColorF m_ClearColor;
				};
			}
		}
	}
}

