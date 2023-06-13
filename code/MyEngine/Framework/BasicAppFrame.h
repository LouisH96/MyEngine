#pragma once

#include <App/Win32/Window.h>

#include "CppFramework.h"
#include "App/FpsControl.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/FocusPointCameraController.h"
#include "Rendering/Canvas.h"
#include "Rendering/Gpu.h"
#include "Game/Globals.h"

namespace MyEngine
{
	namespace Framework
	{
		struct AppServices
		{
			App::Win32::Window& Window;
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
			using namespace Win32;
			using namespace Rendering;
			using namespace Game;

			//BASIC
			CppFramework framework{};
			Window window{ windowName };
			Gpu gpu{ window };
			Globals::pGpu = &gpu;
			Canvas& canvas{ *gpu.MakeCanvas() };

			//CAMERA
			Camera camera{ window.GetClientSize() };
			FocusPointCameraController cameraController{ camera, window.GetKeyboard(), window.GetMouse() };

			//FPS
			FpsControl fpsControl{ 200 };

			//GLOBALS
			Globals::pMouse = &window.GetMouse();
			Globals::pKeyboard = &window.GetKeyboard();
				
			//APP
			T app{ AppServices{window, gpu, canvas, fpsControl, cameraController} };

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
