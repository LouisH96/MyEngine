#include "Drive.h"

using namespace MyEngine;

Drive::Drive(wchar_t letter, Type type)
	: m_Letter{ letter }
	, m_Type{ type }
{
}

std::wstring Drive::GetPath() const
{
	std::wstring path( 2, L':' );
	path[0] = m_Letter;

	return path;
}
