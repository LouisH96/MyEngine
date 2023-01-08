#pragma once
#include "ICameraController.h"

namespace MyEngine
{
	namespace App
	{
		namespace Input
		{
			class Keyboard;
		}
	}
}

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
				CameraController(ICamera& camera, App::Input::Keyboard& keyboard);
				void Update() override;

			private:
				static constexpr float SQR2 = 1.4142135623730950488016887242097f;

				App::Input::Keyboard& m_Keyboard;
				ICamera& m_Camera;
			};
		}
	}
}
