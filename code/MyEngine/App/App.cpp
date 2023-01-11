#include "pch.h"
#include "App.h"

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#include <iostream>

#include "FpsControl.h"
#include "Resources.h"
#include "../Gpu/GpuCreator.h"
#include "../Gpu/ICanvas.h"
#include "../Gpu/IGpu.h"
#include "../Gpu/IMesh.h"
#include "../Gpu/IShader.h"
#include "../Gpu/IPainter.h"
#include "../Logging/Logger.h"
#include "Win32/Window.h"

#include "../Game/Camera/Camera.h"
#include "../Game/Camera/CameraController.h"

void MyEngine::App::App::Run()
{
	using namespace Win32;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Resources::Init();

	Win32::Window& window = *new Win32::Window(L"Window");

	Gpu::IGpu& gpu = *Gpu::GpuCreator::Create(window);

	Gpu::ICanvas& canvas = *gpu.MakeCanvas();
	Gpu::IShader& shader = *gpu.MakeShader();
	Gpu::IMesh& mesh = *gpu.MakeMesh();
	Game::Camera::ICamera& camera = *new Game::Camera::Camera(window.GetSize_WinApi());
	camera.Move({ 0,0,-1 });
	Gpu::IPainter& painter = *gpu.MakePainter();

	painter.SetCanvas(canvas);
	painter.SetShader(shader);
	painter.SetMesh(mesh);
	painter.SetCamera(camera);

	//input
	Game::Camera::CameraController& cameraController = *new Game::Camera::CameraController(camera, window.GetKeyboard());

	//fps 
	FpsControl fpsControl{ 200 };
	
	//loop
	while (!window.IsDestroyed())
	{
		//fps
		fpsControl.Wait();
		Game::GameGlobals::SetDeltaTime(fpsControl.GetDurationLastFrame());

		//window-msg's
		window.HandleMessages();
		if(window.IsResized())
		{
			canvas.OnWindowResized(window.GetSize());
			camera.OnWindowResized(window.GetSize());
		}

		//input
		cameraController.Update();
		camera.Update();
		
		//render
		painter.BeginPaint();
		painter.Paint();
		painter.EndPaint();
	}

	std::cout << "hi\n";

	//input
	delete& cameraController;

	delete& painter;
	delete& camera;
	delete& mesh;
	delete& shader;
	delete& canvas;

	delete& gpu;
	delete& window;
}
