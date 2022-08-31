#include "App.h"

#include <iostream>
#include "../../3rdParty/VLD/include/vld.h"

#include "FpsControl.h"
#include "Resources.h"
#include "../Gpu/GpuCreator.h"
#include "../Gpu/ICanvas.h"
#include "../Gpu/IGpu.h"
#include "../Gpu/IMesh.h"
#include "../Gpu/IShader.h"
#include "../Logging/Logger.h"
#include "Win32/Win32Messages.h"
#include "Win32/Win32Window.h"

void MyEngine::App::App::Run()
{
	Resources::Init();

	IWindow& window = *new Win32::Win32Window(L"Window");
	IOsMessages& osMessages = *new Win32::Win32Messages();

	Gpu::IGpu& gpu = *Gpu::GpuCreator::Create(window);

	Gpu::ICanvas& canvas = *gpu.MakeCanvas(window);
	Gpu::IShader& shader = *gpu.MakeShader();
	Gpu::IMesh& mesh = *gpu.MakeMesh();

	FpsControl fpsControl{ 200 };

	while (!window.IsDestroyed())
	{
		fpsControl.Wait();
		osMessages.HandleMessages();
		window.DispatchEvents();

		gpu.Paint(canvas, shader, mesh);

		//Logging::Logger::Print("Frame " + std::to_string(fpsControl.GetNrFramesLastSec()));
	}

	std::cout << "hi\n";

	delete& mesh;
	delete& shader;
	delete& canvas;

	delete &gpu;
	delete& osMessages;
	delete &window;
}
