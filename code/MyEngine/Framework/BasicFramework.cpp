#include "pch.h"
#include "BasicFramework.h"

#include "Resources.h"

#define _CRTDBG_MAP_ALLOC
#define _LEAK_TO_CONSOLE

MyEngine::App::BasicFramework::BasicFramework()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#ifdef _LEAK_TO_CONSOLE
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
#endif
#endif
	Framework::Resources::Init();
}

MyEngine::App::BasicFramework::~BasicFramework()
{
}
