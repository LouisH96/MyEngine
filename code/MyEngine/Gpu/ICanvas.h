#pragma once
#include <DirectXMath.h>

namespace MyEngine
{
	namespace Gpu
	{
		class ICanvas
		{
		public:
			ICanvas(const ICanvas& other) = delete;
			ICanvas(ICanvas&& other) noexcept = delete;
			ICanvas& operator=(const ICanvas& other) = delete;
			ICanvas& operator=(ICanvas&& other) noexcept = delete;

			ICanvas() = default;
			virtual ~ICanvas() = default;

			virtual void BeginPaint() const = 0;
			virtual void Render() const = 0;

			virtual void Resize(DirectX::XMINT2 newSize) = 0;
		};
	}
}