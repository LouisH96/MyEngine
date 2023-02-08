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

	//MESH
	const Shader::Vertex vertexBuffer[] = {
	   {{0.0f,  0.5f,  0.0f}, {1,0,0}}, // point at top
	   {{0.5f, -0.5f,  0.0f}, {0,1,0}}, // point at bottom-right
	  {{-0.5f, -0.5f,  0.0f}, {0,0,1}}, // point at bottom-left

		{{0.0f, -0.5f, -1.0f}, {1,0,0}},
		{{-0.5f, -0.5f, 0.0f}, {0,1,0}},
		{{0.5f, -0.5f, 0.0f}, {0,0,1}}
	};
	constexpr int indexBuffer[]{ 3,0,1,3,2,0 };

	//APP
	Resources::Init();
	Window& window = *new Window(L"Window");
	Gpu& gpu = *GpuCreator::Create(window);

	Canvas& canvas = *gpu.MakeCanvas();
	Shader& shader = *new Shader(gpu);
	Mesh& mesh = *new Mesh(gpu, vertexBuffer, 6, indexBuffer, 6);
	Game::Camera::Camera& camera = *new Game::Camera::Camera(window.AskClientSize_WinApi());
	camera.Move({ 0,0,-1 });
	Painter& painter = *new Painter();
	
	painter.SetShader(shader);
	painter.SetMesh(mesh);
	painter.SetCamera(camera);

	//input
	Game::Camera::CameraController& cameraController = *new Game::Camera::CameraController(camera, window.GetKeyboard(), window.GetMouse());
	cameraController.SetScrollSpeed(.25f);

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
		canvas.BeginPaint();
		painter.BeginPaint();
		painter.Paint();
		painter.EndPaint();
		canvas.Present();
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
