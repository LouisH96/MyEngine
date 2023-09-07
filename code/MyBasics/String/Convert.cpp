#include "Convert.h"

using namespace MyEngine;

char Convert::ToChar(unsigned oneNumber)
{
	return static_cast<char>('0' + oneNumber);
}
