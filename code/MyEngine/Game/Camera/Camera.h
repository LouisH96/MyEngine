#pragma once
#include <DirectXMath.h>

namespace MyEngine
{
	namespace Game
	{
		namespace Camera
		{
			class Camera
			{
			public:
				explicit Camera(DirectX::XMINT2 size);

				void Update();

				void Move(DirectX::XMFLOAT3 movement);
				void MoveInDirection(DirectX::XMFLOAT3 movement);

				void Pitch(float addAngle);
				void Yaw(float addAngle);

				const DirectX::XMFLOAT4X4& GetViewProjMatrix() const { return m_ViewProjMatrix; }
				void OnWindowResized(DirectX::XMINT2 newSize);
				void SetFieldOfView(float angle);
			private:
				DirectX::XMFLOAT4X4 m_ViewProjMatrix{};
				DirectX::XMFLOAT4X4 m_World{ };
				DirectX::XMFLOAT4X4 m_Proj{};
				float m_FovValue{};
				float m_InvAspectRatio{};

				DirectX::XMFLOAT3 m_FocusPoint{ 0.f,0.f,0.f };
				float m_FocusPointDistance{ 2.f };
				float m_Yaw{};
				float m_Pitch{};

				void UpdateWorldMatrix();
				void UpdateProjMatrix();
				void UpdateViewProjMatrix();
			};
		}
	}
}
