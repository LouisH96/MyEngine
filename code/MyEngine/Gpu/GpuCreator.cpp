#include "GpuCreator.h"

#include "Dx/DxDevice.h"
#include "../App/Win32/Win32Window.h"

MyEngine::Gpu::IGpu* MyEngine::Gpu::GpuCreator::Create(MyEngine::App::IWindow& window)
{
	using namespace App::Win32;
	if (const Win32Window* pWin32 = dynamic_cast<Win32Window*>(&window))
		return new Dx::DxDevice(pWin32->GetWindowHandle());

	return nullptr;
}
