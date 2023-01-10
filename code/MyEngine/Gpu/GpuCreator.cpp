#include "pch.h"
#include "GpuCreator.h"

#include "Dx/DxGpu.h"
#include "../App/Window.h"

MyEngine::Gpu::IGpu* MyEngine::Gpu::GpuCreator::Create(App::Window& window)
{
		return new Dx::DxGpu(window);
}
