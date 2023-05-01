#pragma once
#include "App/Wrappers/Win32/InputTypes.h"
#include "Math/Float4X4.h"

namespace MyEngine
{
	namespace App
	{
		namespace Wrappers
		{
			namespace Win32
			{
				class Mouse;
			}
		}
	}
}

namespace MyEngine
{
	namespace Game
	{
		class Camera;
		class FocusPointCameraController
		{
		public:
			FocusPointCameraController(Camera& camera, const App::Wrappers::Win32::Keyboard& keyboard, const App::Wrappers::Win32::Mouse& mouse);
			void Update();

			void SetFocusPoint(const Math::Float3& position);
			void SetPitch(float pitch);
			void SetYaw(float yaw);
			void SetDistance(float distance);
			void MoveRelative(const Math::Float3& movement);

			void SetScrollSpeed(float scrollSpeed) { m_ScrollSpeed = -scrollSpeed; };
			void SetHorizontalSpeed(float speed) { m_HorizontalSpeed = speed; } //default: 1
			void SetVerticalSpeed(float speed) { m_VerticalSpeed = speed; }

			DirectX::XMFLOAT4X4 GetWorldMatrix() const;
			Math::Float4X4 GetViewMatrix() const;
			Math::Float4X4 GetProjectionMatrix() const;
			Math::Float4X4 GetViewProjectionMatrix() const;
			DirectX::XMMATRIX GetXmViewProjectionMatrix() const;
			Math::Float3 GetCameraPosition() const;

		private:
			const App::Wrappers::Win32::Keyboard& m_Keyboard;
			const App::Wrappers::Win32::Mouse& m_Mouse;
			Camera& m_Camera;
			float m_ScrollSpeed;
			float m_HorizontalSpeed;
			float m_VerticalSpeed;
			Math::Float3 m_FocusPoint;
			float m_Pitch;
			float m_Yaw;
			float m_Distance;

			void KeyboardRotation();
			void MouseRotation();
		};
	}
}
