#pragma once

#include <App/Wrappers/Win32/Window.h>

#include "BasicFramework.h"
#include "App/FpsControl.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/FocusPointCameraController.h"
#include "Rendering/Canvas.h"
#include "Rendering/Gpu.h"

namespace MyEngine
{
	namespace Framework
	{
		struct AppServices
		{
			App::Wrappers::Win32::Window& Window;
			Rendering::Gpu& Gpu;
			Rendering::Canvas& Canvas;
			App::FpsControl& FpsControl;
			Game::FocusPointCameraController& CameraController;
		};

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
			BasicFramework framework{};
			Window window{ windowName };
			Gpu gpu{ window };
			Canvas& canvas{ *gpu.MakeCanvas() };

			//CAMERA
			Camera camera{ window.GetClientSize() };
			FocusPointCameraController cameraController{ camera, window.GetKeyboard(), window.GetMouse() };

			//FPS
			FpsControl fpsControl{ 200 };

			//APP
			T app{ {window, gpu, canvas, fpsControl, camera, cameraController} };

			//LOOP
			while (!window.IsDestroyed())
			{
				//FPS
				fpsControl.NoWait();
				Globals::DeltaTime = fpsControl.GetDurationLastFrame();

				//WINDOW MESSAGES
				window.HandleMessages();
				if (window.IsResized())
				{
					canvas.OnWindowResized(window.GetClientSize());
					camera.OnWindowResized(window.GetClientSize());
				}

				//INPUT
				cameraController.Update();

				//UPDATE
				app.Update();

				//RENDER
				app.Render();
			}

			//DELETE
			app.Release();
			delete& canvas;
		}
	}
}
