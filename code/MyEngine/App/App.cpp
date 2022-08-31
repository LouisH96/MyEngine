#include "App.h"

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
#include "Win32/Win32Messages.h"
#include "Win32/Win32Window.h"

void MyEngine::App::App::Run()
{
	Resources::Init();

	IWindow& window = *new Win32::Win32Window(L"Window");
	IOsMessages& osMessages = *new Win32::Win32Messages();

	Gpu::IGpu& gpu = *Gpu::GpuCreator::Create(window);

	Gpu::ICanvas& canvas = *gpu.MakeCanvas();
	Gpu::IShader& shader = *gpu.MakeShader();
	Gpu::IMesh& mesh = *gpu.MakeMesh();
	Gpu::IPainter& painter = *gpu.MakePainter();

	painter.SetCanvas(canvas);
	painter.SetShader(shader);
	painter.SetMesh(mesh);

	FpsControl fpsControl{ 200 };

	while (!window.IsDestroyed())
	{
		fpsControl.Wait();
		osMessages.HandleMessages();
		window.DispatchEvents();
		
		painter.BeginPaint();
		painter.Paint();
		painter.EndPaint();

		//Logging::Logger::Print("Frame " + std::to_string(fpsControl.GetNrFramesLastSec()));
	}

	std::cout << "hi\n";

	delete& painter;
	delete& mesh;
	delete& shader;
	delete& canvas;

	delete &gpu;
	delete& osMessages;
	delete &window;
}
