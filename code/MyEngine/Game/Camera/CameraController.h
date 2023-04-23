#pragma once
#include "App/Wrappers/Win32/InputTypes.h"

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
		class FocusPointCamera;

		class CameraController
		{
		public:
			CameraController(FocusPointCamera& camera, const App::Wrappers::Win32::Keyboard& keyboard, const App::Wrappers::Win32::Mouse& mouse);
			void Update() const;

			void SetScrollSpeed(float scrollSpeed) { m_ScrollSpeed = -scrollSpeed; };
			void SetHorizontalSpeed(float speed) { m_HorizontalSpeed = speed; } //default: 1
			void SetVerticalSpeed(float speed) { m_VerticalSpeed = speed; }

		private:
			const App::Wrappers::Win32::Keyboard& m_Keyboard;
			const App::Wrappers::Win32::Mouse& m_Mouse;
			FocusPointCamera& m_Camera;
			float m_ScrollSpeed;
			float m_HorizontalSpeed;
			float m_VerticalSpeed;

			void KeyboardRotation() const;
			void MouseRotation() const;
		};
	}
}
