#include "pch.h"
#include "CoreServices.h"

Framework::CoreServices::CoreServices(App::Wrappers::Win32::Window& window, Rendering::Gpu& gpu,
	Rendering::Canvas& canvas, App::FpsControl& fpsControl)
	: m_Window{ window }
	, m_Gpu{ gpu }
	, m_Canvas{ canvas }
	, m_FpsControl{ fpsControl }
{
}
