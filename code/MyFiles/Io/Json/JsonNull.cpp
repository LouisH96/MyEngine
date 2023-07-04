#include "JsonNull.h"

#include <fstream>

MyEngine::Io::Json::JsonNull::JsonNull(std::ifstream& file)
{
	file.seekg(4, std::ios_base::cur);
}

std::string MyEngine::Io::Json::JsonNull::ToString() const
{
	return "null";
}

bool MyEngine::Io::Json::JsonNull::DetectType(char c)
{
	return c == 'n';
}
