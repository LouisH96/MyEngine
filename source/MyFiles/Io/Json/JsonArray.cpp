#include "JsonArray.h"

#include <fstream>
#include <sstream>

#include "Logger/Logger.h"

MyEngine::Io::Json::JsonArray::JsonArray(std::ifstream& stream)
{
	const char first = stream.get();
	if (first != '[')
	{
		Logger::Error("first char in JsonArray should be [ instead of " + first + '\n');
		return;
	}
	char c;
	while (stream.get(c))
	{
		if (c == ' ') continue;
		if (c == '\n') continue;
		if (c == ',') continue;
		if (c == ']') return;
		stream.seekg(-1, std::ios_base::cur);
		const auto nextType = FindElementType(stream);
		if (nextType == ElementType::Unknown)
		{
			Logger::Error("unknown state in array");
			return;
		}
		auto pElement = ReadElement(stream);
		m_Elements.push_back(pElement);
	}
}

MyEngine::Io::Json::JsonArray::~JsonArray()
{
	for (const auto pElement : m_Elements)
		delete pElement;
}

std::string MyEngine::Io::Json::JsonArray::ToString() const
{
	std::stringstream ss{};
	ss << "[\n";
	const auto last = std::prev(m_Elements.cend());
	for(auto it = m_Elements.cbegin(); it != m_Elements.cend(); ++it)
	{
		ss << '\t' << (*it)->ToString();
		if (it != last)
			ss << ",\n";
	}
	ss << "\n\t]";
	return ss.str();
}

bool MyEngine::Io::Json::JsonArray::DetectType(char c)
{
	return c == '[';
}
