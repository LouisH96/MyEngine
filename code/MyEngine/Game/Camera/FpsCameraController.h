#pragma once
#include "App/Wrappers/Win32/Window.h"
#include "Game/Transform.h"

namespace MyEngine
{
	namespace Game
	{
		class Camera;

		class FpsCameraController
		{
		public:
			FpsCameraController(const Camera& camera,
				const App::Wrappers::Win32::Window::Keyboard& keyboard,
				const App::Wrappers::Win32::Mouse& mouse);
			void Update();

			DirectX::XMMATRIX GetProjectionMatrix() const;
			DirectX::XMMATRIX GetWorldMatrix() const;
			DirectX::XMMATRIX GetViewMatrix() const;
			DirectX::XMMATRIX GetViewProjectionMatrix() const;
			Math::Float3 GetCameraPosition() const;
			Game::Transform GetTransform() const;

			void SetLookAroundSpeed(Math::Float2 speed) { m_LookAroundSpeed = speed; }
			void SetPosition(const Math::Float3& position) { m_Position = position; }
			void MoveRelative(const Math::Float3& movement);

		private:
			const App::Wrappers::Win32::Window::Keyboard& m_Keyboard;
			const App::Wrappers::Win32::Mouse& m_Mouse;
			const Camera& m_Camera;
			Math::Float3 m_Position;
			Math::Float2 m_LookAroundSpeed;
			float m_Pitch;
			float m_Yaw;
		};
	}
}

