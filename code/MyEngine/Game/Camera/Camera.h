#pragma once

#include "Game/Transform.h"
#include "Math/Int2.h"

namespace MyEngine
{
	namespace Game
	{
		class Camera
		{
		public:
			explicit Camera(Math::Int2 windowSize, float fov = 90, float near = .01f, float far = 50);

			DirectX::XMFLOAT4X4 GetViewProjectionMatrix() const;
			void OnWindowResized(Math::Int2 windowSize);
			void SetFieldOfView(float angle);

			const Transform& GetTransform() const { return m_Transform; }
			Transform& GetTransform() { return m_Transform; }

			void SetPosition(const Math::Float3& position);

		private:
			Transform m_Transform{};
			DirectX::XMFLOAT4X4 m_ProjectionMatrix{};
			float m_InvAspectRatio{};
			float m_FovValue{};
			float m_Near;
			float m_Far;

			void UpdateProjectionMatrix();
		};
	}
}
