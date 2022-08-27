#include "App.h"

#include <iostream>

#include "Win32MsgHandler.h"
#include "Win32Window.h"
#include "../Framework/Dx/DxFramework.h"

void MyEngine::App::App::Run()
{

	const Win32Window window(L"Window");
	Win32MsgHandler msgQueue{};
	Framework::Dx::DxFramework ff{ window.GetWindowHandle() };

	while (msgQueue.IsActive())
	{
		msgQueue.HandleMessages();
	}

	std::cout << "hi\n";
}
