#include "pch.h"
#include "Globals.h"

float Globals::TimeScale{ 1.f };
float Globals::Time{ 0 };
float Globals::DeltaTime{ 0 };
float Globals::UnscaledTime{ 0 };
float Globals::UnscaledDeltaTime{ 0 };
App::Win32::Window* Globals::pWindow = nullptr;
const App::Win32::Keyboard_MsgListener* Globals::pKeyboard = nullptr;
App::Win32::Mouse* Globals::pMouse = nullptr;
Rendering::Gpu* Globals::pGpu = nullptr;
const Rendering::Canvas* Globals::pCanvas = nullptr;
Camera* Globals::pCamera = nullptr;
Applied::Camera2D* Globals::pCamera2D = nullptr;
Ui::UiSystem* Globals::pUi{};
