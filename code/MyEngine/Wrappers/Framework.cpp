#include "pch.h"
#include "Framework.h"

void MyEngine::Wrappers::Framework::Init()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
}
