#pragma once
#include "ICamera.h"

namespace MyEngine
{
	namespace Game
	{
		namespace Camera
		{
			class Camera final
				: public ICamera
			{
			public:
				explicit Camera(DirectX::XMINT2 size);
				void Move(DirectX::XMFLOAT3 movement) override;

				const DirectX::XMFLOAT4X4& GetMatrix() const { return m_Matrix; }
				void OnWindowResized(DirectX::XMINT2 newSize) override;
				void SetFieldOfView(float angle) override;
			private:
				DirectX::XMFLOAT4X4 m_Matrix{1,0,0,0, 0,1,0,0, 0,0,1,1 ,0,0,0,0};
				float m_FovValue{};
				float m_InvAspectRatio{};
			};
		}
	}
}
