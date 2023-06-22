#pragma once
#include "CppFramework.h"
#include "App/FpsControl.h"
#include "App/Win32/Window.h"
#include "Game/Camera/Camera.h"
#include "Rendering/Canvas.h"
#include "Rendering/FpsDisplay.h"
#include "Rendering/Gpu.h"

namespace MyEngine
{
	namespace Framework
	{
		class GameServices
		{
		public:
			GameServices(App::Win32::Window& window,
				Rendering::Canvas& canvas,
				App::FpsControl& fpsControl,
				Camera& camera);

			App::Win32::Window& Window;
			Rendering::Canvas& Canvas;
			App::FpsControl& FpsControl;
			Camera& Camera;
		};

		class GameFrame
		{
		public:
			template<typename T>
			static void Run(const std::wstring& windowName = L"Window", App::Win32::Window::Options options = {});
		};

		template <typename T>
		void GameFrame::Run(const std::wstring& windowName, App::Win32::Window::Options options)
		{
			using namespace App;
			using namespace Win32;
			using namespace Rendering;
			using namespace Game;

			//BASIC
			CppFramework framework{};
			Window window{ windowName, options };
			Gpu gpu{ window };
			Canvas canvas{ window };

			//CAMERA
			Camera camera{ window.GetClientSize() };
			Globals::pCamera = &camera;

			//FPS
			FpsDisplay fpsDisplay{ };
			FpsControl fpsControl{ 200, &fpsDisplay };

			//GAME
			T app{  };

			//LOOP
			while (!window.IsDestroyed())
			{
				//FPS
				fpsControl.NoWait();

				//WINDOW MESSAGES
				window.HandleMessages();
				if (window.IsResized())
				{
					canvas.OnWindowResized(window.GetClientSize());
					app.OnWindowResized(window.GetClientSize());
					camera.OnWindowResized(window.GetClientSize());
				}

				//UPDATE
				app.EarlyUpdate();
				camera.Update();
				app.Update();

				//RENDER
				canvas.BeginPaint();
				app.Render();
				fpsDisplay.Render();
				canvas.Present();
			}
		}
	}
}
