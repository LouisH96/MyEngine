#pragma once

namespace MyEngine
{
	namespace NewUi
	{
		class NewUiFontRenderer;
		class Root;
	}

	namespace SimpleUi
	{
		class SimpleUiSystem;
	}

	namespace Applied
	{
		class Camera2D;
	}

	namespace Gui
	{
		class GuiRenderer;
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
	using Ui = NewUi::Root;

	class Globals
	{
	public:
		static float DeltaTime;
		static App::Win32::Window* pWindow;
		static const Keyboard* pKeyboard;
		static App::Win32::Mouse* pMouse;

		static Rendering::Gpu* pGpu;
		static const Rendering::Canvas* pCanvas;
		static Game::Camera* pCamera;
		static Applied::Camera2D* pCamera2D;

		static Gui::GuiRenderer* pGuiRenderer;
		static Gui::FontRenderer* pFontRenderer;

		static NewUi::NewUiFontRenderer* pNewFontRenderer;
		static Ui* pUi;
	};
}

#define DELTA_TIME Globals::DeltaTime
#define WINDOW (*Globals::pWindow)
#define KEYBOARD (*Globals::pKeyboard)
#define MOUSE (*Globals::pMouse)
#define GPU (*Globals::pGpu)
#define CANVAS (*Globals::pCanvas)
#define CAMERA (*Globals::pCamera)
#define CAMERA_2D (*Globals::pCamera2D)
#define GUI (*Globals::pGuiRenderer)
#define FONT (*Globals::pFontRenderer)
#define UI (*Globals:pUi)
#define NEW_FONT (*Globals::pNewFontRenderer)