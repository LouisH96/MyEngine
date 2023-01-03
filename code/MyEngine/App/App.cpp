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
#include "Win32/Win32Window.h"

#include "Input/InputData.h"
#include "Input/InputWriter.h"
#include "Input/InputReader.h"

#include "../Game/Camera/Camera.h"
#include "../Game/Camera/CameraController.h"

void MyEngine::App::App::Run()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Resources::Init();

	IWindow& window = *new Win32::Win32Window(L"Window");

	Gpu::IGpu& gpu = *Gpu::GpuCreator::Create(window);

	Gpu::ICanvas& canvas = *gpu.MakeCanvas();
	Gpu::IShader& shader = *gpu.MakeShader();
	Gpu::IMesh& mesh = *gpu.MakeMesh();
	Game::Camera::ICamera& camera = *new Game::Camera::Camera(window.GetSize());
	window.Listen(camera);
	camera.Move({ 0,0,-1 });
	Gpu::IPainter& painter = *gpu.MakePainter();

	painter.SetCanvas(canvas);
	painter.SetShader(shader);
	painter.SetMesh(mesh);
	painter.SetCamera(camera);

	//input
	Input::IInputData& inputData = *new Input::InputData();
	Input::IInputWriter& inputWriter = *inputData.CreateWriter();
	Input::IInputReader& inputReader = *inputData.CreateReader();
	window.SetInputWriter(inputWriter);
	Game::Camera::CameraController& cameraController = *new Game::Camera::CameraController(camera, inputReader);

	//fps 
	FpsControl fpsControl{ 200 };

	//loop
	while (!window.IsDestroyed())
	{
		//fps
		fpsControl.Wait();
		Game::GameGlobals::SetDeltaTime(fpsControl.GetDurationLastFrame());

		//input
		inputData.Update();
		cameraController.Update();
		camera.Update();

		//update
		window.HandleMessages();

		//render
		painter.BeginPaint();
		painter.Paint();
		painter.EndPaint();

		//Logging::Logger::Print("Frame " + std::to_string(fpsControl.GetNrFramesLastSec()));
	}

	std::cout << "hi\n";

	//input
	delete& cameraController;
	delete& inputReader;
	delete& inputWriter;
	delete& inputData;

	delete& painter;
	delete& camera;
	delete& mesh;
	delete& shader;
	delete& canvas;

	delete& gpu;
	delete& window;
}
