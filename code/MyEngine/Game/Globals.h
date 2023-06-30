#pragma once
#include "App/Win32/Window.h"
#include "App/Win32/Keyboard_MsgListener.h"
#include "App/Win32/Mouse.h"

namespace MyEngine
{
	namespace Game
	{
		class Camera;
	}
	namespace Rendering
	{
		class Canvas;
		class Gpu;
	}

	using Keyboard = App::Win32::Window::Keyboard;

	class Globals
	{
	public:

		static float DeltaTime;
		static App::Win32::Window* pWindow;
		static const App::Win32::Keyboard_MsgListener* pKeyboard;
		static const App::Win32::Mouse* pMouse;

		static Rendering::Gpu* pGpu;
		static const Rendering::Canvas* pCanvas;
		static Game::Camera* pCamera;
	};
}
