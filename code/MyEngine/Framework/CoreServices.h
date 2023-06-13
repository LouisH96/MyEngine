#pragma once

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

	namespace Framework
	{
		class CoreServices
		{
		public:
			CoreServices(App::Win32::Window& window,
				Rendering::Canvas& canvas,
				App::FpsControl& fpsControl,
				Camera& camera);

			App::Win32::Window& Window;
			Rendering::Canvas& Canvas;
			App::FpsControl& FpsControl;
			Camera& Camera;
		};
	}
}

