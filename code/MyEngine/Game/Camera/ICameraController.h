#pragma once

namespace MyEngine
{
	namespace App
	{
		namespace Input
		{
			class IInputReader;
		}
	}
}

namespace MyEngine
{
	namespace Game
	{
		namespace Camera
		{
			class ICameraController
			{
			public:
				ICameraController(const ICameraController& other) = delete;
				ICameraController(ICameraController&& other) noexcept = delete;
				ICameraController& operator=(const ICameraController& other) = delete;
				ICameraController& operator=(ICameraController&& other) noexcept = delete;

				ICameraController() = default;
				virtual ~ICameraController() = default;

				virtual void Update(float dt) = 0;
			};
		}
	}
}
