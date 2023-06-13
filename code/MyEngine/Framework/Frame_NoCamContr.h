#pragma once

#include <App/Win32/Window.h>

#include "CppFramework.h"
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
		class Frame_NoCamContr
		{
		public:
			static void Run(const std::wstring& windowName = L"Window", App::Win32::Window::Options options = {});
		};

		template <typename T>
		void Frame_NoCamContr<T>::Run(const std::wstring& windowName, App::Win32::Window::Options options)
		{
			using namespace App;
			using namespace Win32;
			using namespace Rendering;
			using namespace Game;

			//BASIC
			CppFramework framework{};
			Window window{ windowName, options };
			Gpu gpu{ window };
			Globals::pGpu = &gpu;
			Canvas& canvas{ *gpu.MakeCanvas() };
			Globals::pKeyboard = &window.GetKeyboard();
			Globals::pMouse = &window.GetMouse();

			//FPS
			FpsControl fpsControl{ 200 };

			//APP
			Camera camera{ window.GetClientSize() };
			T app{ CoreServices{window, gpu, canvas, fpsControl, camera} };

			//LOOP
			while (!window.IsDestroyed())
			{
				//FPS
				fpsControl.NoWait();
				Globals::DeltaTime = fpsControl.GetDurationLastFrame();

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
