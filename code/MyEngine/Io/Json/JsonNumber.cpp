#include "pch.h"
#include "JsonNumber.h"

#include <fstream>

Io::Json::JsonNumber::JsonNumber(std::ifstream& file)
	: m_Number(0)
{
	double isNeg = 1;
	double decimal = 0;
	char c;
	while (file.get(c))
	{
		if (c == '-')
			isNeg = -1;
		else if (c == '.')
			decimal = 10;
		else if (c == 'f')
			break;
		else if (c >= '0' && c <= '9')
		{
			if (decimal > 0)
			{
				m_Number += (c - '0') / decimal;
				decimal *= 10;
			}
			else
			{
				m_Number *= 10;
				m_Number += c - '0';
			}
		}
		else
		{
			file.seekg(-1, std::ios_base::cur);
			break;
		}
	}
	while (decimal > 1)
		decimal /= 10;
	m_Number += decimal;
	m_Number *= isNeg;
}

std::string Io::Json::JsonNumber::ToString() const
{
	return std::to_string(m_Number);
}

bool Io::Json::JsonNumber::DetectType(char c)
{
	if (c >= '0' && c <= '9') return true;
	if (c == '-') return true;
	if (c == '.') return true;
	return false;
}
