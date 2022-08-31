#pragma once
#include <DirectXMath.h>

namespace MyEngine
{
	namespace App
	{
		class IWindowResizeListener
		{
		public:
			IWindowResizeListener(const IWindowResizeListener& other) = delete;
			IWindowResizeListener(IWindowResizeListener&& other) noexcept = delete;
			IWindowResizeListener& operator=(const IWindowResizeListener& other) = delete;
			IWindowResizeListener& operator=(IWindowResizeListener&& other) noexcept = delete;

			IWindowResizeListener() = default;
			virtual ~IWindowResizeListener() = default;

			virtual void OnWindowResized(DirectX::XMINT2 newSize) = 0;
		};
	}
}
