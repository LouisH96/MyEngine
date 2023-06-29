#pragma once

namespace MyEngine
{
	namespace Game
	{
		class Camera;
		class OrbitCamera
		{
		public:
			OrbitCamera();
			explicit OrbitCamera(Camera& camera);
			void Update();

			void SetFocusPoint(const Float3& position);
			void SetPitch(float pitch);
			void SetYaw(float yaw);
			void SetDistance(float distance);
			void MoveRelative(const Float3& movement);

			void SetScrollSpeed(float scrollSpeed) { m_ScrollSpeed = -scrollSpeed; };
			void SetHorizontalSpeed(float speed) { m_HorizontalSpeed = speed; } //default: 1
			void SetVerticalSpeed(float speed) { m_VerticalSpeed = speed; }

			Float3 GetCameraPosition() const;
			const Camera& GetCamera() const { return m_Camera; }

		private:
			Camera& m_Camera;
			float m_ScrollSpeed;
			float m_HorizontalSpeed;
			float m_VerticalSpeed;
			Float3 m_FocusPoint;
			float m_Pitch;
			float m_Yaw;
			float m_Distance;

			void MouseRotation();
		};
	}
}
