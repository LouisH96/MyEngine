#pragma once

#include <App/Wrappers/Win32/Window.h>

#include "BasicFramework.h"
#include "CoreServices.h"
#include "App/FpsControl.h"
#include "Rendering/Canvas.h"
#include "Rendering/Gpu.h"
#include <Game/Camera/Camera.h>

namespace MyEngine
{
	namespace Framework
	{
		template <typename T>
		class Frame_NoCam
		{
		public:
			static void Run(const std::wstring& windowName = L"Window", App::Wrappers::Win32::Window::Options options = {});
		};

		template <typename T>
		void Frame_NoCam<T>::Run(const std::wstring& windowName, App::Wrappers::Win32::Window::Options options)
		{
			using namespace App;
			using namespace Wrappers::Win32;
			using namespace Rendering;
			using namespace Game;

			//BASIC
			BasicFramework framework{};
			Window window{ windowName, options };
			Gpu gpu{ window };
			Canvas& canvas{ *gpu.MakeCanvas() };

			//FPS
			FpsControl fpsControl{ 200 };

			//APP
			Camera camera{ window.GetClientSize() };
			T app{ CoreServices{window, gpu, canvas, camera, fpsControl} };

			//LOOP
			while (!window.IsDestroyed())
			{
				//FPS
				fpsControl.NoWait();
				GameGlobals::SetDeltaTime(fpsControl.GetDurationLastFrame());

				//WINDOW MESSAGES
				window.HandleMessages();
				if (window.IsResized())
					canvas.OnWindowResized(window.GetClientSize());

				//APP
				app.Update();
				app.Render();
			}

			//DELETE
			app.Release();
			delete& canvas;
		}
	}
}
