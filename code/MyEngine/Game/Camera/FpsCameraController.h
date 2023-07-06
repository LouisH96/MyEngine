#pragma once

namespace MyEngine
{
	namespace Game
	{
		class Camera;

		class FpsCameraController
		{
		public:
			FpsCameraController();
			explicit FpsCameraController(Camera& camera);
			void Update();

			void SetLookAroundSpeed(Float2 speed) { m_LookAroundSpeed = speed; }
			void SetPosition(const Float3& position) { m_Position = position; }
			void SetPositionY(float y) { m_Position.y = y; }
			void SetPositionXz(const Float2& position);;
			void MoveRelative(const Float3& movement);
			Float2 GetXzForward() const;
			Float3 GetXzForward3() const;

			const Camera& GetCamera() const { return m_Camera; }
			const Float3& GetPosition() const { return m_Position; }
			Transform GetTransform() const;

		private:
			Camera& m_Camera;
			Float3 m_Position;
			Float2 m_LookAroundSpeed;
			float m_Pitch;
			float m_Yaw;
		};
	}
}
