#include "pch.h"
#include "BasicExampleApp.h"

#include <iostream>
#include <Framework/Resources.h>
#include <Rendering/Gpu.h>
#include <Rendering/State/Mesh.h>
#include <Rendering/State/Shader.h>
#include "FpsControl.h"
#include "../Game/Camera/FocusPointCamera.h"
#include "../Game/Camera/CameraController.h"
#include "App/Wrappers/Win32/Window.h"
#include "DataStructures/Array.h"
#include "Generation/Shapes.h"
#include "Math/Cube.h"
#include "Math/Float3.h"
#include "Rendering/Canvas.h"
#include "Rendering/State/ConstantBuffer.h"
#include "Rendering/State/InputLayout.h"

using namespace Rendering;

void App::BasicExampleApp::Run()
{
	using namespace Wrappers::Win32;
	using namespace Ds;
	using namespace Math;

	//APP
	Window& window = *new Window(L"Window");
	Gpu& gpu = *new Gpu(window);
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

	Rendering::Shader& shader = *new Rendering::Shader(gpu, Framework::Resources::GetGlobalShaderPath(L"unlit.hlsl"));
	Rendering::Mesh& mesh = *Rendering::Mesh::Create<Vertex>(gpu, cubeVertices, cubeIndices);
	const ConstantBuffer<CameraMatrixCBuffer> constantBuffer{ gpu };

	//GAME
	Game::FocusPointCamera& camera = *new Game::FocusPointCamera(window.AskClientSize_WinApi());

	//RENDER-PIPELINE
	constantBuffer.ActivateVs(gpu);
	inputLayout.Activate(gpu);
	shader.Activate();
	mesh.Activate(gpu);

	//input
	Game::CameraController& cameraController = *new Game::CameraController(camera, window.GetKeyboard(), window.GetMouse());
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
		shader.Activate();
		mesh.Activate(gpu);
		mesh.Draw(gpu);
		camera.Update();
		canvas.Present();
	}

	std::cout << "hi\n";

	//input
	delete& cameraController;

	delete& camera;
	delete& mesh;
	delete& shader;
	delete& canvas;

	delete& gpu;
	delete& window;
}
