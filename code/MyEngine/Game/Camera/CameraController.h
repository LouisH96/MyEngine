#pragma once
#include "ICameraController.h"
#include "App/Win32/InputTypes.h"

namespace MyEngine
{
	namespace Game
	{
		namespace Camera
		{
			class ICamera;

			class CameraController final
				: public ICameraController
			{
			public:
				CameraController(ICamera& camera, const App::Win32::Keyboard& keyboard);
				void Update() override;

			private:
				const App::Win32::Keyboard& m_Keyboard;
				ICamera& m_Camera;
			};
		}
	}
}
