#include "pch.h"
#include "CoreServices.h"

Framework::CoreServices::CoreServices(App::Win32::Window& window, Rendering::Gpu& gpu,
	Rendering::Canvas& canvas, Game::Camera& camera, App::FpsControl& fpsControl)
	: m_Window{ window }
	, m_Gpu{ gpu }
	, m_Canvas{ canvas }
	, m_FpsControl{ fpsControl }
	, m_Camera{ camera }
{
}
