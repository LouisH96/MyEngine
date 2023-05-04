#pragma once
#include "Rendering/Gpu.h"

namespace MyEngine
{
	namespace Game
	{
		class Camera;
	}

	namespace App
	{
		class FpsControl;
		namespace Win32
		{
			class Window;
		}
	}
	namespace Rendering
	{
		class Canvas;
	}
}

namespace MyEngine
{
	namespace Framework
	{
		class CoreServices
		{
		public:
			CoreServices(App::Win32::Window& window,
				Rendering::Gpu& gpu,
				Rendering::Canvas& canvas,
				App::FpsControl& fpsControl,
				Game::Camera& camera);

			App::Win32::Window& Window;
			Rendering::Gpu& Gpu;
			Rendering::Canvas& Canvas;
			App::FpsControl& FpsControl;
			Game::Camera& Camera;
		};
	}
}

