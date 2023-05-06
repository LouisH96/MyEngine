#pragma once
#include "Math/Float4X4.h"
#include "Math/Int2.h"

namespace MyEngine
{
	namespace Game
	{
		class Camera
		{
		public:
			explicit Camera(Math::Int2 windowSize, float fov = 90, float near = .01f, float far = 200);

			void OnWindowResized(Math::Int2 windowSize);
			void SetFieldOfView(float angle);
			const DirectX::XMFLOAT4X4& GetDxProjectionMatrix() const { return m_DxProjectionMatrix; }
			const DirectX::XMMATRIX& GetXmProjectionMatrix() const;
			const Math::Float4X4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

		private:
			DirectX::XMFLOAT4X4 m_DxProjectionMatrix{};
			Math::Float4X4 m_ProjectionMatrix{};
			float m_InvAspectRatio{};
			float m_FovValue{};
			float m_Near;
			float m_Far;

			void UpdateProjectionMatrix();
		};
	}
}
