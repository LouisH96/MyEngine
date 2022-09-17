#include "pch.h"
#include "GpuCreator.h"

#include "Dx/DxGpu.h"
#include "../App/Win32/Win32Window.h"

MyEngine::Gpu::IGpu* MyEngine::Gpu::GpuCreator::Create(MyEngine::App::IWindow& window)
{
	using namespace App::Win32;
	if (Win32Window* pWin32 = dynamic_cast<Win32Window*>(&window))
		return new Dx::DxGpu(*pWin32);

	return nullptr;
}
