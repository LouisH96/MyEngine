#include "pch.h"
#include "BasicExampleApp.h"

#include <iostream>

#include "FpsControl.h"
#include "Resources.h"
#include "App/Wrappers/Dx/GpuCreator.h"
#include "App/Wrappers/Dx/Canvas.h"
#include "App/Wrappers/Dx/Gpu.h"
#include "App/Wrappers/Dx/Mesh.h"
#include "App/Wrappers/Dx/Shader.h"
#include "App/Wrappers/Dx/Painter.h"
#include "App/Wrappers/Win32/Window.h"

#include "../Game/Camera/Camera.h"
#include "../Game/Camera/CameraController.h"
#include "App/Wrappers/Dx/InputLayout.h"
#include "DataStructures/Array.h"
#include "Generation/Shapes.h"
#include "Math/Cube.h"
#include "Math/Float3.h"
#include "Wrappers/Dx/ConstantBuffer.h"

void MyEngine::App::BasicExampleApp::Run()
{
	using namespace Wrappers::Win32;
	using namespace Wrappers::Dx;
	using namespace Dx;
	using namespace Ds;
	using namespace Math;

	//APP
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
	//cube
	Array<Float3> cubePositions{};
	Array<Float3> cubeNormals{};
	Array<int> cubeIndices{};
	const Cube cube{ {0,0,0},1.f };
	Generation::Shapes::GenerateCubeBuffers(cube, cubePositions, cubeNormals, cubeIndices);
	Array<Vertex> cubeVertices{ cubePositions.GetSize() };
	for (int i = 0; i < cubeVertices.GetSize(); i++)
	{
		DirectX::XMFLOAT3 color;
		if (i / 8 == 0) color = { 1,0,0 };
		else if (i / 8 == 1) color = { 0,1,0 };
		else color = { 0,0,1 };
		cubeVertices[i] = { {cubePositions[i].x, cubePositions[i].y, cubePositions[i].z }, color };
	}
	const InputLayout::Element meshElements[]
	{
		{"POSITION", InputLayout::ElementType::Float3},
		{"COLOR", InputLayout::ElementType::Float3},
	};
	const InputLayout inputLayout{ gpu, meshElements, ARRAYSIZE(meshElements) };

	Shader& shader = *new Shader(gpu, Resources::GetGlobalShaderPath(L"shader.hlsl"));
	Mesh<Vertex>& mesh = *new Mesh<Vertex>(gpu, cubeVertices.GetData(), cubeVertices.GetSize(), cubeIndices.GetData(), cubeIndices.GetSize());
	const ConstantBuffer<CameraMatrixCBuffer> constantBuffer{ gpu };

	//GAME
	Game::Camera::Camera& camera = *new Game::Camera::Camera(window.AskClientSize_WinApi());
	Painter<Vertex>& painter = *new Painter<Vertex>();

	//RENDER-PIPELINE
	constantBuffer.ActivateVs(gpu);
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
