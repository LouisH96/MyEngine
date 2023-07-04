#include "JsonObject.h"

#include <fstream>
#include <sstream>

#include "JsonString.h"
#include "Logger/Logger.h"

MyEngine::Io::Json::JsonObject::JsonObject(std::ifstream& stream)
{
	const char first = stream.get();
	if (first != '{')
	{
		Logger::PrintError("first char in JsonObject should be { instead of " + first + '\n');
		return;
	}

	std::string propertyName;
	while (FindNextProperty(stream, propertyName))
	{
		JsonElement* pElement = ReadElement(stream);
		m_Properties[propertyName] = pElement;

		if (!FindComma(stream)) break;
	}
}

MyEngine::Io::Json::JsonObject::~JsonObject()
{
	for (auto it = m_Properties.begin(); it != m_Properties.end(); ++it)
		delete it->second;
}

const MyEngine::Io::Json::JsonElement& MyEngine::Io::Json::JsonObject::GetElementProp(const std::string& propertyName) const
{
	return *m_Properties.at(propertyName);
}

const MyEngine::Io::Json::JsonObject& MyEngine::Io::Json::JsonObject::GetObjectProp(const std::string& propertyName) const
{
	return m_Properties.at(propertyName)->AsObject();
}

const MyEngine::Io::Json::JsonArray& MyEngine::Io::Json::JsonObject::GetArrayProp(const std::string& propertyName) const
{
	return m_Properties.at(propertyName)->AsArray();
}

const std::string& MyEngine::Io::Json::JsonObject::GetStringProp(const std::string& propertyName) const
{
	return m_Properties.at(propertyName)->AsString().Get();
}

std::string MyEngine::Io::Json::JsonObject::ToString() const
{
	if (m_Properties.empty())
		return "{}\n";
	std::stringstream ss{};

	ss << "{\n";
	const auto last = std::prev(m_Properties.cend());
	for (auto it = m_Properties.cbegin(); it != m_Properties.cend(); ++it)
	{
		ss << "\t\"";
		ss << it->first << "\" : ";
		ss << it->second->ToString();
		if (it != last)
			ss << ',';
		ss << std::endl;
	}
	ss << "}\n";
	return ss.str();
}

bool MyEngine::Io::Json::JsonObject::DetectType(char c)
{
	return c == '{';
}

bool MyEngine::Io::Json::JsonObject::FindNextProperty(std::ifstream& file, std::string& propertyName)
{
	propertyName.clear();
	std::streampos begin = 0;
	char c;
	while (file.get(c))
	{
		if (c == '"')
		{
			if (begin == 0)
			{
				begin = file.tellg();
			}
			else if (propertyName.empty())
			{
				const auto size = file.tellg() - begin;
				const auto pChar = new char[size];
				file.seekg(begin, std::ios_base::beg);
				file.get(pChar, size);
				file.seekg(1, std::ios_base::cur);
				propertyName = { pChar };
				delete[] pChar;
			}
			else
				Logger::PrintError("Should not encounter another \"");
		}
		else if (c == ':')
		{
			if (propertyName.empty())
				Logger::PrintError(": found before property-name");
			else
				return true;
		}
		else if (c == '}')
			return false;
		else if (c == ',')
		{
			Logger::PrintError("Unexpected ,");
		}
	}
	Logger::PrintError("invalid property");
	return false;
}

bool MyEngine::Io::Json::JsonObject::FindComma(std::ifstream& stream)
{
	char c;
	while (stream.get(c))
	{
		if (c == ',') return true;
		if (c == '}') return false;
	}

	Logger::PrintError("FindComma function failed");
	return false;
}
