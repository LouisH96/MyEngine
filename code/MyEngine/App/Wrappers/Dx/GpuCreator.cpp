#include "pch.h"
#include "GpuCreator.h"

#include "Gpu.h"
#include "App/Wrappers/Win32/Window.h"

MyEngine::App::Wrappers::Dx::Gpu* MyEngine::App::Wrappers::Dx::GpuCreator::Create(App::Wrappers::Win32::Window& window)
{
	return new Gpu(window);
}
