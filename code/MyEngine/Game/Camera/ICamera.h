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

				virtual void Update() = 0;

				virtual void Move(DirectX::XMFLOAT3 translation) = 0;
				virtual void MoveInDirection(DirectX::XMFLOAT3 movement) = 0;

				virtual void Pitch(float addAngle) = 0;
				virtual void Yaw(float addAngle) = 0;

				virtual void OnWindowResized(DirectX::XMINT2 newSize) = 0;
				virtual void SetFieldOfView(float angle) = 0;
			};
		}
	}
}
