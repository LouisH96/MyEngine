#pragma once
#undef min
#undef max

#include <cmath>
#include <iostream>

#include <Math\Matrices.h>
#include <Math\Scalars.h>
#include <Math\Vectors.h>

#include <Framework\Globals.h>
#include <Logger\Logger.h>

#include <App\Win32\Keyboard_MsgListener.h>
#include <App\Win32\Mouse.h>
#include <App\Win32\KeyCode.h>
#include <DataStructures\Array.h>
#include <DataStructures\List.h>
#include <DataStructures\Pointers\SharedPtr.h>
#include <Debug\Rendering\DebugRenderer.h>
#include <Framework\Resources.h>
#include <Other\Color.h>
#include <Timing\AvgCounter.h>
#include <Timing\Counter.h>
#include <Transform\Transform.h>

using namespace MyEngine;
using namespace Debug;
using namespace Ui;
using namespace Timing;
