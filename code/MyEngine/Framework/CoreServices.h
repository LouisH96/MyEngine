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
				Rendering::Gpu& gpu, Rendering::Canvas& canvas,
				Game::Camera& camera,
				App::FpsControl& fpsControl);

			App::Win32::Window& GetWindow() const { return m_Window; }
			Rendering::Gpu& GetGpu() const { return m_Gpu; }
			Rendering::Canvas& GetCanvas() const { return m_Canvas; }
			App::FpsControl& GetFpsControl() const { return m_FpsControl; }
			Game::Camera& GetCamera() const { return m_Camera; }

		private:
			App::Win32::Window& m_Window;
			Rendering::Gpu& m_Gpu;
			Rendering::Canvas& m_Canvas;
			App::FpsControl& m_FpsControl;
			Game::Camera& m_Camera;
		};
	}
}

