#pragma once

namespace MyEngine
{
	namespace Gui
	{
		class FontRenderer;
	}

	namespace App
	{
		namespace Win32
		{
			class Mouse;
			class Window;
			class Keyboard_MsgListener;
		}
	}

	namespace Game
	{
		class Camera;
	}
	namespace Rendering
	{
		class Canvas;
		class Gpu;
	}

	using Keyboard = App::Win32::Keyboard_MsgListener;

	class Globals
	{
	public:

		static float DeltaTime;
		static App::Win32::Window* pWindow;
		static const Keyboard* pKeyboard;
		static const App::Win32::Mouse* pMouse;

		static Rendering::Gpu* pGpu;
		static const Rendering::Canvas* pCanvas;
		static Game::Camera* pCamera;

		static Gui::FontRenderer* pFontRenderer;
	};
}
