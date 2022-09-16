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

				void Update() override;

				void Move(DirectX::XMFLOAT3 movement) override;
				void MoveInDirection(DirectX::XMFLOAT3 movement) override;

				void Pitch(float addAngle) override;
				void Yaw(float addAngle) override;

				const DirectX::XMFLOAT4X4& GetViewProjMatrix() const { return m_ViewProjMatrix; }
				void OnWindowResized(DirectX::XMINT2 newSize) override;
				void SetFieldOfView(float angle) override;
			private:
				DirectX::XMFLOAT4X4 m_ViewProjMatrix{ 1,0,0,0, 0,1,0,0, 0,0,1,1 ,0,0,0,0 };
				DirectX::XMFLOAT4X4 m_World{ 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
				DirectX::XMFLOAT4X4 m_Proj{ 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
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
