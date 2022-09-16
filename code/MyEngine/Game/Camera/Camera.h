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
				void Pitch(float addAngle) override;
				void Yaw(float addAngle) override;

				const DirectX::XMFLOAT4X4& GetMatrix() const { return m_Matrix; }
				void OnWindowResized(DirectX::XMINT2 newSize) override;
				void SetFieldOfView(float angle) override;
			private:
				DirectX::XMFLOAT4X4 m_Matrix{ 1,0,0,0, 0,1,0,0, 0,0,1,1 ,0,0,0,0 };
				DirectX::XMFLOAT4X4 m_World{ 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
				DirectX::XMFLOAT4X4 m_Proj{ 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
				float m_FovValue{};
				float m_InvAspectRatio{};

				float m_Yaw{};
				float m_Pitch{};
				float m_FocusPointDistance{ 2.f };

				void UpdateRotationMatrix();
			};
		}
	}
}
