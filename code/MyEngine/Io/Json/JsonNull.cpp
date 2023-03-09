#include "pch.h"
#include "JsonNull.h"

#include <fstream>

Io::Json::JsonNull::JsonNull(std::ifstream& file)
{
	file.seekg(4, std::ios_base::cur);
}

std::string Io::Json::JsonNull::ToString() const
{
	return "null";
}

bool Io::Json::JsonNull::DetectType(char c)
{
	return c == 'n';
}
