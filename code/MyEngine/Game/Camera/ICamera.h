#pragma once
#include <DirectXMath.h>
#include "../../Gpu/ICanvas.h"


namespace MyEngine
{
	namespace Game
	{
		namespace Camera
		{
			class ICamera
				: public App::IWindowResizeListener
			{
			public:
				ICamera(const ICamera& other) = delete;
				ICamera(ICamera&& other) noexcept = delete;
				ICamera& operator=(const ICamera& other) = delete;
				ICamera& operator=(ICamera&& other) noexcept = delete;

				ICamera() = default;
				virtual ~ICamera() override = default;

				virtual void Move(DirectX::XMFLOAT3 translation) = 0;
				virtual void OnWindowResized(DirectX::XMINT2 newSize) override = 0;
			};
		}
	}
}
