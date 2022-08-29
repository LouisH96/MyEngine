#include "App.h"

#include <iostream>
#include "../../3rdParty/VLD/include/vld.h"

#include "Win32MsgHandler.h"
#include "Win32Window.h"
#include "../DirectX/DxDevice.h"
#include "FpsControl.h"
#include "../Logging/Logger.h"

void MyEngine::App::App::Run()
{
	const Win32Window window(L"Window");
	Win32MsgHandler msgQueue{};
	DirectX::DxDevice ff{ window.GetWindowHandle() };

	FpsControl fpsControl{ 80 };

	while (!window.IsDestroyed())
	{
		msgQueue.HandleMessages();
		fpsControl.Wait();
		ff.Render();
		Logging::Logger::Print("Frame " + std::to_string(fpsControl.GetNrFramesLastSec()));
	}

	std::cout << "hi\n";
}
