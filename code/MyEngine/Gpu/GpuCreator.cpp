#include "pch.h"
#include "GpuCreator.h"

#include "Dx/DxGpu.h"
#include "../App/Win32/Window.h"

MyEngine::Gpu::IGpu* MyEngine::Gpu::GpuCreator::Create(App::Win32::Window& window)
{
		return new Dx::DxGpu(window);
}
