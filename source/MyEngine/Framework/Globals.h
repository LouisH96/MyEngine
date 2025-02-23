#pragma once

namespace MyEngine
{
class Camera;

namespace Ui
{
class UiSystem;
class UiFontRenderer;
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
	static App::Win32::Mouse* pMouse;

	static Rendering::Gpu* pGpu;
	static const Rendering::Canvas* pCanvas;
	static Camera* pCamera;
	static Applied::Camera2D* pCamera2D;

	static Ui::UiSystem* pUi;
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

#define UI (*Globals::pUi)
#define UI_TREE (UI.GetTreeManager())
#define UI_RECT (UI.GetRectRenderer())
#define UI_SHAPE (UI.GetShapeRenderer())
#define UI_IMAGE (UI.GetImageRenderer())
#define UI_FONT (Globals::pUi->GetFontRenderer())