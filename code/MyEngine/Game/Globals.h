#pragma once
#include "App/Win32/Keyboard_MsgListener.h"
#include "App/Win32/Mouse.h"

namespace MyEngine
{
	namespace Game
	{
		class Camera;
	}

	namespace App
	{
		namespace Win32
		{
			class Window;
		}
	}

	namespace Rendering
	{
		class Canvas;
		class Gpu;
	}

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
