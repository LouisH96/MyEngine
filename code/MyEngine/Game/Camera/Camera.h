#pragma once
#include "Math/Int2.h"

namespace MyEngine
{
	namespace Game
	{
		class Camera
		{
		public:
			explicit Camera(Math::Int2 windowSize, float fov = 90, float near = .01f, float far = 50);

			void OnWindowResized(Math::Int2 windowSize);
			void SetFieldOfView(float angle);
			const DirectX::XMFLOAT4X4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
			const DirectX::XMMATRIX& GetXmProjectionMatrix() const;

		private:
			DirectX::XMFLOAT4X4 m_ProjectionMatrix{};
			float m_InvAspectRatio{};
			float m_FovValue{};
			float m_Near;
			float m_Far;

			void UpdateProjectionMatrix();
		};
	}
}
