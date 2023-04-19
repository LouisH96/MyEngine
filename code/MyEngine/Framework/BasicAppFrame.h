#pragma once

#include <App/Wrappers/Win32/Window.h>

#include "App/FpsControl.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/CameraController.h"
#include "Rendering/Canvas.h"
#include "Rendering/Gpu.h"

namespace MyEngine
{
	namespace Framework
	{
		template<typename T>
		class BasicAppFrame
		{
		public:
			static void Run(const std::wstring& windowName = L"Window");
		};

		template <typename T>
		void BasicAppFrame<T>::Run(const std::wstring& windowName)
		{
			using namespace App;
			using namespace Wrappers::Win32;
			using namespace Rendering;
			using namespace Game;

			//BASIC
			Window window{ windowName };
			Gpu gpu{ window };
			Canvas& canvas{ *gpu.MakeCanvas() };

			//CAMERA
			Camera camera{ window.AskClientSize_WinApi() };
			CameraController cameraController{ camera, window.GetKeyboard(), window.GetMouse() };

			//FPS
			FpsControl fpsControl{ 200 };

			//APP
			T app{ window, gpu, canvas, camera, fpsControl };

			//LOOP
			while(!window.IsDestroyed())
			{
				//FPS
				fpsControl.NoWait();
				GameGlobals::SetDeltaTime(fpsControl.GetDurationLastFrame());

				//WINDOW MESSAGES
				window.HandleMessages();
				if(window.IsResized())
				{
					canvas.OnWindowResized(window.GetClientSize());
					camera.OnWindowResized(window.GetClientSize());
				}

				//INPUT
				cameraController.Update();
				camera.Update();

				app.Update();
			}

			//DELETE
			app.Release();
			delete& canvas;
		}
	}
}
