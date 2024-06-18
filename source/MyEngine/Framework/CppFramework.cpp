#include "pch.h"
#include "CppFramework.h"

#include "Resources.h"

#define _CRTDBG_MAP_ALLOC
#define _LEAK_TO_CONSOLE

Framework::CppFramework::CppFramework()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#ifdef _LEAK_TO_CONSOLE
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
#endif
#endif
	Resources::Init();
	Logger::SetColorWhite();
}

Framework::CppFramework::~CppFramework()
{
}
