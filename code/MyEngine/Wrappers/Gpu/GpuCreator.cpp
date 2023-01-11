#include "pch.h"
#include "GpuCreator.h"

#include "Gpu.h"
#include "Wrappers/Win32/Window.h"

MyEngine::Wrappers::Gpu::Gpu* MyEngine::Wrappers::Gpu::GpuCreator::Create(Wrappers::Win32::Window& window)
{
		return new Gpu(window);
}
