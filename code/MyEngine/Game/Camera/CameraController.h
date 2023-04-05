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
		class Camera;

		class CameraController
		{
		public:
			CameraController(Camera& camera, const App::Wrappers::Win32::Keyboard& keyboard, const App::Wrappers::Win32::Mouse& mouse);
			void Update();

			void SetScrollSpeed(float scrollSpeed) { m_ScrollSpeed = -scrollSpeed; };
			void SetHorizontalSpeed(float speed) { m_HorizontalSpeed = speed; } //default: 1

		private:
			const App::Wrappers::Win32::Keyboard& m_Keyboard;
			const App::Wrappers::Win32::Mouse& m_Mouse;
			Camera& m_Camera;
			float m_ScrollSpeed;
			float m_HorizontalSpeed;

			void KeyboardRotation() const;
			void MouseRotation() const;
		};
	}
}
