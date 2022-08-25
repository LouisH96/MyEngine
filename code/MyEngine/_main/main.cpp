#include <iostream>
#include "../Framework/Win32Framework.h"
#include "..\Framework\Dx\DxFramework.h"

int main()
{
	MyEngine::Framework::Win32Framework f( L"Window" );
	MyEngine::Framework::Dx::DxFramework ff{f.GetWindowHandle()};

	while(f.Loop())
	{
		
	}

	std::cout << "hi\n";
	return 0;
}
