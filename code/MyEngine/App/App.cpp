#include "pch.h"
#include "App.h"

#include <iostream>

#include "FpsControl.h"
#include "Resources.h"
#include "App/Wrappers/Gpu/GpuCreator.h"
#include "App/Wrappers/Gpu/Canvas.h"
#include "App/Wrappers/Gpu/Gpu.h"
#include "App/Wrappers/Gpu/Mesh.h"
#include "App/Wrappers/Gpu/Shader.h"
#include "App/Wrappers/Gpu/Painter.h"
#include "../Logging/Logger.h"
#include "App/Wrappers/Win32/Window.h"

#include "../Game/Camera/Camera.h"
#include "../Game/Camera/CameraController.h"

void MyEngine::App::App::Run()
{
	using namespace Wrappers::Win32;
	using namespace Wrappers::Gpu;

	m_Framework.Init();
	Resources::Init();

	Window& window = *new Window(L"Window");

	Gpu& gpu = *GpuCreator::Create(window);

	Canvas& canvas = *gpu.MakeCanvas();
	Shader& shader = *gpu.MakeShader();
	Mesh& mesh = *gpu.MakeMesh();
	Game::Camera::Camera& camera = *new Game::Camera::Camera(window.AskClientSize_WinApi());
	camera.Move({ 0,0,-1 });
	Painter& painter = *gpu.MakePainter();

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
			canvas.OnWindowResized(window.GetClientSize());
			camera.OnWindowResized(window.GetClientSize());
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
