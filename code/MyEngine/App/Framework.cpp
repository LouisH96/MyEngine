#include "pch.h"
#include "Framework.h"

#define _CRTDBG_MAP_ALLOC

void MyEngine::App::Framework::Init()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
}
