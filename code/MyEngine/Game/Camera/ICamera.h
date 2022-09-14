#pragma once
#include <DirectXMath.h>


namespace MyEngine
{
	namespace Game
	{
		namespace Camera
		{
			class ICamera
			{
			public:
				ICamera(const ICamera& other) = delete;
				ICamera(ICamera&& other) noexcept = delete;
				ICamera& operator=(const ICamera& other) = delete;
				ICamera& operator=(ICamera&& other) noexcept = delete;

				ICamera() = default;
				virtual ~ICamera() = default;

				virtual void Move(DirectX::XMFLOAT3 translation) = 0;
			};
		}
	}
}
