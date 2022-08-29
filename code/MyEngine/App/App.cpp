#include "App.h"

#include <iostream>

#include "Win32MsgHandler.h"
#include "Win32Window.h"
#include "../Framework/Dx/DxFramework.h"
#include "FpsControl.h"
#include "../Logging/Logger.h"

void MyEngine::App::App::Run()
{
	const Win32Window window(L"Window");
	Win32MsgHandler msgQueue{};
	Framework::Dx::DxFramework ff{ window.GetWindowHandle() };

	FpsControl fpsControl{ 60 };

	while (msgQueue.IsActive())
	{
		msgQueue.HandleMessages();
		fpsControl.Wait();
		Logging::Logger::Print("Frame " + std::to_string(fpsControl.GetNrFramesLastSec()));
	}

	std::cout << "hi\n";
}
