#include "pch.h"
#include "Globals.h"

float Globals::DeltaTime = 0.f;
const App::Win32::Keyboard_MsgListener* Globals::pKeyboard = nullptr;
const App::Win32::Mouse* Globals::pMouse = nullptr;
Rendering::Gpu* Globals::pGpu = nullptr;
