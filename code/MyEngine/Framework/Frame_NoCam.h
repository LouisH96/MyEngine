#pragma once

#include <App/Wrappers/Win32/Window.h>

#include "BasicFramework.h"
#include "CoreServices.h"
#include "App/FpsControl.h"
#include "Rendering/Canvas.h"
#include "Rendering/Gpu.h"

namespace MyEngine
{
	namespace Framework
	{
		template <typename T>
		class Frame_NoCam
		{
		public:
			static void Run(const std::wstring& windowName = L"Window");
		};

		template <typename T>
		void Frame_NoCam<T>::Run(const std::wstring& windowName)
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

			//FPS
			FpsControl fpsControl{ 200 };

			//APP
			T app{ CoreServices{window, gpu, canvas, fpsControl} };

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
