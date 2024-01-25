#include "pch.h"
#include "Globals.h"

float Globals::DeltaTime = 0.f;
App::Win32::Window* Globals::pWindow = nullptr;
const App::Win32::Keyboard_MsgListener* Globals::pKeyboard = nullptr;
App::Win32::Mouse* Globals::pMouse = nullptr;
Rendering::Gpu* Globals::pGpu = nullptr;
const Rendering::Canvas* Globals::pCanvas = nullptr;
Camera* Globals::pCamera = nullptr;
Applied::Camera2D* Globals::pCamera2D = nullptr;
Gui::GuiRenderer* Globals::pGuiRenderer = nullptr;
Gui::FontRenderer* Globals::pFontRenderer = nullptr;
Ui* Globals::pUi{};
NewUi::NewUiFontRenderer* Globals::pNewFontRenderer{};