#pragma once
#include "ICameraController.h"

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
				CameraController(ICamera& camera, App::Input::IInputReader& input);
				void Update() override;

			private:
				static constexpr float SQR2 = 1.4142135623730950488016887242097f;

				App::Input::IInputReader& m_Input;
				ICamera& m_Camera;
			};
		}
	}
}
