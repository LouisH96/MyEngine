#pragma once
#include "CppFramework.h"
#include "App/FpsControl.h"
#include "App/Win32/Window.h"
#include "Applied/NewUi/NewUiFontRenderer.h"
#include "Applied/NewUi/NewUiSystem.h"
#include "Debug/Rendering/DebugRenderer.h"
#include "Camera/Camera.h"
#include "Other\Random.h"
#include "Rendering/Canvas.h"
#include "Rendering/FpsDisplay.h"
#include "Rendering/Gpu.h"

namespace MyEngine
{
	namespace Framework
	{
		class GameFrame
		{
		public:
			struct GameFrameOptions
			{
				bool RandomSeed{ true };
			};

			template<typename T>
			static void Run(const std::wstring& windowName = L"Window", App::Win32::Window::Options options = {}, const GameFrameOptions& gameFrameOptions = {});
		};

		template <typename T>
		void GameFrame::Run(const std::wstring& windowName, App::Win32::Window::Options options, const GameFrameOptions& gameFrameOptions)
		{
			using namespace App;
			using namespace Win32;
			using namespace Rendering;

			//BASIC
			CppFramework framework{};
			Window window{ windowName, options };
			Gpu gpu{ };
			Canvas canvas{ window, options.BackgroundColor };

			if (gameFrameOptions.RandomSeed)
				Random::Seed();

			//CAMERA
			Camera camera{ window.GetClientSize() };
			Globals::pCamera = &camera;

			//RENDERING
			DebugRenderer::Init();
			Globals::pUi = new NewUi::NewUiSystem(canvas.GetSize());

			//FPS
			FpsDisplay fpsDisplay{};
			FpsControl fpsControl{ 60, &fpsDisplay };

			//GAME
			T* pApp{ new T() };

			//LOOP
			fpsControl.Reset();
			while (!window.IsDestroyed())
			{
				//FPS
				fpsControl.NoWait();

				//WINDOW MESSAGES
				window.HandleMessages();
				if (window.IsResized())
				{
					const ResizedEvent resizedEvent{ canvas.OnWindowResized(window.GetClientSize()) };

					Globals::pUi->OnCanvasResized(resizedEvent);

					camera.OnCanvasResized(resizedEvent);
					pApp->OnCanvasResized(resizedEvent);
				}

				//UPDATE
				pApp->EarlyUpdate();
				camera.Update();
				pApp->Update();
				Globals::pUi->Update();

				//RENDER
				pApp->PreRender();
				canvas.BeginPaint();
				pApp->Render();
				DebugRenderer::Render();
				canvas.ClearDepthBuffer();
				pApp->RenderUi();
				Globals::pUi->Render();
				fpsDisplay.Render();
				canvas.Present();
			}

			delete pApp;
			delete Globals::pUi;
			DebugRenderer::Release();
		}
	}
}
