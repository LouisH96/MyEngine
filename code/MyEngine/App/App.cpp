#include "pch.h"
#include "App.h"

#include <iostream>

#include "FpsControl.h"
#include "Resources.h"
#include "App/Wrappers/Dx/GpuCreator.h"
#include "App/Wrappers/Dx/Canvas.h"
#include "App/Wrappers/Dx/Gpu.h"
#include "App/Wrappers/Dx/Mesh.h"
#include "App/Wrappers/Dx/Shader.h"
#include "App/Wrappers/Dx/Painter.h"
#include "../Logging/Logger.h"
#include "App/Wrappers/Win32/Window.h"

#include "../Game/Camera/Camera.h"
#include "../Game/Camera/CameraController.h"
#include "App/Wrappers/Dx/InputLayout.h"
#include "Wrappers/Dx/ConstantBuffer.h"

void MyEngine::App::App::Run()
{
	using namespace Wrappers::Win32;
	using namespace Wrappers::Dx;
	using namespace Dx;

	//APP
	Resources::Init();
	Window& window = *new Window(L"Window");
	Gpu& gpu = *GpuCreator::Create(window);
	Canvas& canvas = *gpu.MakeCanvas();

	//MESH
	struct Vertex
	{
		DirectX::XMFLOAT3 pos{};
		DirectX::XMFLOAT3 col{};
	};
	struct CameraMatrixCBuffer
	{
		DirectX::XMFLOAT4X4 cameraMatrix;
	};
	const Vertex vertexBuffer[] = {
	   {{0.0f,  0.5f,  0.0f}, {1,0,0}}, // point at top
	   {{0.5f, -0.5f,  0.0f}, {0,1,0}}, // point at bottom-right
	  {{-0.5f, -0.5f,  0.0f}, {0,0,1}}, // point at bottom-left

		{{0.0f, -0.5f, -1.0f}, {1,0,0}},
		{{-0.5f, -0.5f, 0.0f}, {0,1,0}},
		{{0.5f, -0.5f, 0.0f}, {0,0,1}}
	};
	constexpr int indexBuffer[]{ 3,0,1,3,2,0 };
	const InputLayout::Element meshElements[]
	{
		{"POSITION", InputLayout::ElementType::Float3},
		{"COLOR", InputLayout::ElementType::Float3},
	};
	const InputLayout inputLayout{ gpu, meshElements, ARRAYSIZE(meshElements) };

	Shader<Vertex>& shader = *new Shader<Vertex>(gpu, Resources::GetGlobalShaderPath(L"shader.hlsl"));
	Mesh<Vertex>& mesh = *new Mesh<Vertex>(gpu, vertexBuffer, 6, indexBuffer, 6);
	const ConstantBuffer<CameraMatrixCBuffer> constantBuffer{ gpu };

	//GAME
	Game::Camera::Camera& camera = *new Game::Camera::Camera(window.AskClientSize_WinApi());
	camera.Move({ 0,0,-1 });
	Painter<Vertex>& painter = *new Painter<Vertex>();

	//RENDER-PIPELINE
	constantBuffer.Activate(gpu);
	inputLayout.Activate(gpu);
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
		if (window.IsResized())
		{
			canvas.OnWindowResized(window.GetClientSize());
			camera.OnWindowResized(window.GetClientSize());
		}

		//input
		cameraController.Update();
		camera.Update();

		//render
		constantBuffer.Update(gpu, { camera.GetViewProjMatrix() });
		canvas.BeginPaint();
		painter.Paint();
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
