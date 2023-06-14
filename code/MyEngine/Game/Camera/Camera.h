#pragma once

namespace MyEngine
{
	namespace Game
	{
		class Camera
		{
		public:
			explicit Camera(Int2 windowSize, float fov = 90, float near = .01f, float far = 200);

			void OnWindowResized(Int2 windowSize);
			void Update();

			void SetFieldOfView(float angle);

			void SetPosition(const Float3& position);

			void SetRight(const Float3& right);
			void SetUp(const Float3& up);
			void SetForward(const Float3& forward);
			void SetRotation(float pitch, float yaw);

			const Float3& GetRight() const;
			const Float3& GetUp() const;
			const Float3& GetForward() const;

			const Float4X4& GetProjection() const { return m_Projection; }
			const Float4X4& GetWorld() const { return m_World; }
			const Float4X4& GetView() const { return m_View; }
			const Float4X4& GetViewProjection() const { return m_ViewProjection; }

			Float3 GetPosition() const;

		private:
			Float4X4 m_Projection{};
			Float4X4 m_World{};
			Float4X4 m_View{};
			Float4X4 m_ViewProjection{};

			float m_InvAspectRatio{};
			float m_FovValue{};
			float m_Near;
			float m_Far;

			void UpdateProjectionMatrix();
		};
	}
}
