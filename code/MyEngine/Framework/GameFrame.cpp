#include "pch.h"
#include "GameFrame.h"

Framework::GameServices::GameServices(App::Win32::Window& window, Rendering::Canvas& canvas,
	App::FpsControl& fpsControl, Game::Camera& camera)
	: Window{ window }
	, Canvas{ canvas }
	, FpsControl{ fpsControl }
	, Camera{ camera }
{
}
