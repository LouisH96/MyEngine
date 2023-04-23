#pragma once
#include <DirectXMath.h>
#include <Math/Float3.h>

namespace MyEngine
{
	namespace Game
	{
		class FocusPointCamera
		{
		public:
			explicit FocusPointCamera(DirectX::XMINT2 size);

			void Update();

			void Move(DirectX::XMFLOAT3 movement);
			void MoveInDirection(DirectX::XMFLOAT3 movement);
			void SetFocusPoint(const Math::Float3& focus);

			void Pitch(float addAngle);
			void Yaw(float addAngle);
			void Zoom(float addDistance);

			const DirectX::XMFLOAT4X4& GetViewProjMatrix() const { return m_ViewProjMatrix; }
			float GetFocusPointDistance() const { return m_FocusPointDistance; }
			void OnWindowResized(DirectX::XMINT2 newSize);
			void SetFieldOfView(float angle);
			DirectX::XMFLOAT3 GetPosition() const;
			Math::Float3 GetPositionFloat3() const;

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
