#pragma once
#include "ICameraController.h"
#include "App/Input/InputTypes.h"

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
				CameraController(ICamera& camera, const App::Input::Keyboard& keyboard);
				void Update() override;

			private:
				const App::Input::Keyboard& m_Keyboard;
				ICamera& m_Camera;
			};
		}
	}
}
