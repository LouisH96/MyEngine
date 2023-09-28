#pragma once
#include "CppFramework.h"
#include "App/FpsControl.h"
#include "Game/Camera/Camera.h"
#include "Debug/Rendering/DebugRenderer.h"
#include "Gui/FontRenderer.h"
#include "Gui/GuiRenderer.h"
#include "Rendering/Canvas.h"
#include "Rendering/FpsDisplay.h"
#include "Rendering/Gpu.h"
#include "App/Win32/Window.h"

namespace MyEngine
{
	namespace Framework
	{
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
			Canvas canvas{ window, options.BackgroundColor };

			//CAMERA
			Camera camera{ window.GetClientSize() };
			Globals::pCamera = &camera;

			//RENDERING
			DebugRenderer::Init();
			Globals::pGuiRenderer = new Gui::GuiRenderer();
			Globals::pFontRenderer = new Gui::FontRenderer();

			//FPS
			FpsDisplay fpsDisplay{};
			FpsControl fpsControl{ 1000, &fpsDisplay };

			//GAME
			T app{};

			//LOOP
			fpsControl.Reset();
			while (!window.IsDestroyed())
			{
				//FPS
				fpsControl.Wait();

				//WINDOW MESSAGES
				window.HandleMessages();
				if (window.IsResized())
				{
					const ResizedEvent resizedEvent{ canvas.OnWindowResized(window.GetClientSize()) };
					camera.OnCanvasResized(resizedEvent);
					Globals::pGuiRenderer->OnCanvasResized(resizedEvent);
					Globals::pFontRenderer->OnCanvasResized(resizedEvent);
					app.OnCanvasResized(resizedEvent);
				}

				//UPDATE
				app.EarlyUpdate();
				camera.Update();
				app.Update();

				//RENDER
				canvas.BeginPaint();
				app.Render();
				DebugRenderer::Render();
				canvas.ClearDepthBuffer();
				app.RenderUi();
				Globals::pGuiRenderer->Render();
				Globals::pFontRenderer->Render();
				fpsDisplay.Render();
				canvas.Present();
			}

			delete Globals::pGuiRenderer;
			delete Globals::pFontRenderer;
			DebugRenderer::Release();
		}
	}
}
