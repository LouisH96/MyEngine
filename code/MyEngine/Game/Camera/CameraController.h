#pragma once
#include "Wrappers/Win32/InputTypes.h"

namespace MyEngine
{
	namespace Game
	{
		namespace Camera
		{
			class Camera;

			class CameraController
			{
			public:
				CameraController(Camera& camera, const Wrappers::Win32::Keyboard& keyboard);
				void Update();

			private:
				const Wrappers::Win32::Keyboard& m_Keyboard;
				Camera& m_Camera;
			};
		}
	}
}
