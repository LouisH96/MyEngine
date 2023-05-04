#include "pch.h"
#include "CoreServices.h"

Framework::CoreServices::CoreServices(App::Win32::Window& window, Rendering::Gpu& gpu, Rendering::Canvas& canvas,
	App::FpsControl& fpsControl, Game::Camera& camera)
	: Window{ window }
	, Gpu{ gpu }
	, Canvas{ canvas }
	, FpsControl{ fpsControl }
	, Camera{ camera }
{
}
