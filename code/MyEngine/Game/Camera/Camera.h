#pragma once

namespace MyEngine
{
	namespace App
	{
		struct ResizedEvent;
	}

	namespace Game
	{
		class Camera
		{
		public:
			explicit Camera(Int2 windowSize, float fov = 60, float nearPlane = .01f, float farPlane = 200);

			void OnCanvasResized(const App::ResizedEvent& event);
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

			const Float3& GetPosition() const { return m_Position; }
			float GetHalfFov() const; //in radians
			float GetTanHalfFov() const { return m_TanHalfFov; }

		private:
			Float4X4 m_Projection{};
			Float4X4 m_World{};
			Float4X4 m_View{};
			Float4X4 m_ViewProjection{};
			Float3 m_Position{};

			float m_AspectRatio{};
			float m_TanHalfFov{};
			float m_Near;
			float m_Far;

			void UpdateProjectionMatrix();
		};
	}
}
