#include "JsonNumber.h"

#include <fstream>
#include <iostream>

MyEngine::Io::Json::JsonNumber::JsonNumber(std::ifstream& file)
	: m_Number(Read(file))
{
}

std::string MyEngine::Io::Json::JsonNumber::ToString() const
{
	return std::to_string(m_Number);
}

bool MyEngine::Io::Json::JsonNumber::DetectType(char c)
{
	if (c >= '0' && c <= '9') return true;
	if (c == '-') return true;
	if (c == '.') return true;
	return false;
}

double MyEngine::Io::Json::JsonNumber::Read(std::ifstream& stream)
{
	double number = 0;
	double isNeg = 1;
	char c;
	while (stream.get(c))
	{
		if (stream.eof())
			std::cout << "eof\n";

		if (!stream.good())
			std::cout << "not good\n";

		if (c >= '0' && c <= '9')
		{
			number *= 10;
			number += c - '0';
		}
		else if (c == '-')
			isNeg = -1;
		else if (c == '.')
		{
			break;
		}
		else if (c == 'f')
			break;
		else
		{
			stream.seekg(-1, std::ios_base::cur);
			break;
		}
	}
	if (c == '.')
	{
		double decimalPart = 0;
		double decimalScale = 1;
		while (stream.get(c))
		{
			if (c >= '0' && c <= '9')
			{
				decimalPart *= 10;
				decimalPart += c - '0';
				decimalScale *= 10;
			}
			else if (c == 'f')
				break;
			else if (c == 'e')
				break;
			else
			{
				stream.seekg(-1, std::ios_base::cur);
				break;
			}
		}
		number += decimalPart / decimalScale;
	}
	if (c == 'e')
	{
		double exponent{ 0 };
		char sign{};
		stream.get(sign);

		if (sign == '-')
			exponent *= -1;

		while (stream.get(c))
		{
			if (c >= '0' && c <= '9')
			{
				exponent *= 10;
				exponent += static_cast<double>(c - '0');
			}
			else
				break;
		}

		number *= pow(10, exponent);
	}

	number *= isNeg;
	return number;
}
