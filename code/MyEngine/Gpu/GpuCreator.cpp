#include "pch.h"
#include "GpuCreator.h"

#include "Dx/DxGpu.h"
#include "../Wrappers/Win32/Window.h"

MyEngine::Gpu::IGpu* MyEngine::Gpu::GpuCreator::Create(Wrappers::Win32::Window& window)
{
		return new Dx::DxGpu(window);
}
