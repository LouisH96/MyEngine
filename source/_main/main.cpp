#include <iostream>
#include "../Framework/Win32Framework.h"

int main()
{
	MyEngine::Framework::Win32Framework f( L"Window" );
	std::cout << "hi\n";
	return 0;
}
