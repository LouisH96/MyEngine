#include "JsonString.h"

#include <fstream>
#include <sstream>

#include "Logger/Logger.h"

MyEngine::Io::Json::JsonString::JsonString(std::ifstream& stream)
{
	const char first = stream.get();
	if (first != '"')
	{
		std::stringstream ss{};
		ss << "[JsonString::JsonString] first char in JsonString should be \" instead of ";
		ss << first << std::endl;
		Logger::Error(ss.str());
		return;
	}
	const std::streampos begin = stream.tellg();
	char c;
	while (stream.get(c))
	{
		if (c != '"') continue;
		const auto size = stream.tellg() - begin;
		const auto pChar = new char[size];
		stream.seekg(begin, std::ios_base::beg);
		stream.get(pChar, size);
		stream.seekg(1, std::ios_base::cur);
		m_Value = { pChar };
		delete[] pChar;
		return;
	}
}

std::string MyEngine::Io::Json::JsonString::ToString() const
{
	return '"' + m_Value + '"';
}

bool MyEngine::Io::Json::JsonString::DetectType(char c)
{
	return c == '"';
}
