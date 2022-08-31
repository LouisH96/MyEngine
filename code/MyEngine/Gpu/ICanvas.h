#pragma once
#include <DirectXMath.h>
#include "../App/IWindowResizeListener.h"

namespace MyEngine
{
	namespace Gpu
	{
		class ICanvas
			: public App::IWindowResizeListener
		{
		public:
			ICanvas(const ICanvas& other) = delete;
			ICanvas(ICanvas&& other) noexcept = delete;
			ICanvas& operator=(const ICanvas& other) = delete;
			ICanvas& operator=(ICanvas&& other) noexcept = delete;

			ICanvas() = default;
			~ICanvas() override = default;

			virtual void BeginPaint() const = 0;
			virtual void Render() const = 0;

			void OnWindowResized(DirectX::XMINT2 newSize) override = 0;
		};
	}
}