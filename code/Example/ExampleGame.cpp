#include <iostream>

#include "../MyEngine/App/Win32Window.h"
#include "../MyEngine/App/Win32MsgHandler.h"
#include "../MyEngine/Framework/Dx/DxFramework.h"

int main()
{
	using namespace MyEngine;

	const App::Win32Window window(L"Window");
	App::Win32MsgHandler msgQueue{};
	Framework::Dx::DxFramework ff{ window.GetWindowHandle() };

	while (msgQueue.IsActive())
	{
		msgQueue.HandleMessages();
	}

	std::cout << "hi\n";
	return 0;
}