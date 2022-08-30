#include "App.h"

#include <iostream>
#include "../../3rdParty/VLD/include/vld.h"

#include "FpsControl.h"
#include "Win32MsgHandler.h"
#include "Win32Window.h"
#include "../DirectX/DxDevice.h"
#include "../Logging/Logger.h"
#include "../DirectX/GpuCreator.h"

void MyEngine::App::App::Run()
{
	IWindow& window = *new Win32Window(L"Window");
	Win32MsgHandler msgQueue{};
	DirectX::DxDevice& gpu = *DirectX::GpuCreator::Create(window);

	FpsControl fpsControl{ 80 };

	while (!window.IsDestroyed())
	{
		fpsControl.Wait();
		msgQueue.HandleMessages();
		window.DispatchEvents();
		gpu.Render();
		//Logging::Logger::Print("Frame " + std::to_string(fpsControl.GetNrFramesLastSec()));
	}

	std::cout << "hi\n";

	delete& window;
	delete &gpu;
}
