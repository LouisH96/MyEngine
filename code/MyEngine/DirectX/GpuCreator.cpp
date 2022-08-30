#include "GpuCreator.h"

#include "DxDevice.h"
#include "../App/Win32Window.h"
#include "../Logging/Logger.h"

MyEngine::DirectX::DxDevice* MyEngine::DirectX::GpuCreator::Create(App::IWindow& window)
{
	using namespace App;
	if (const Win32Window* pWin32 = dynamic_cast<Win32Window*>(&window))
		return new DxDevice(pWin32->GetWindowHandle());

	return nullptr;
}
