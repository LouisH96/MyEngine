#include "JsonBool.h"

#include <String\Convert.h>
#include <fstream>

using namespace MyEngine::Io::Json;

JsonBool::JsonBool(std::ifstream& file)
	: m_Value{ Read(file) }
{
}

std::string JsonBool::ToString() const
{
	return Convert::ToString(m_Value);
}

bool JsonBool::DetectType(char c)
{
	return c == 't' || c == 'f' || c == 'T' || c == 'F';
}

bool JsonBool::Read(std::ifstream& stream)
{
	char c;
	stream.get(c);

	if (c == 't' || c == 'T')
		stream.seekg(3, std::ios_base::cur); //true - 1
	else
		stream.seekg(4, std::ios_base::cur); //false -1

	return true;
}
