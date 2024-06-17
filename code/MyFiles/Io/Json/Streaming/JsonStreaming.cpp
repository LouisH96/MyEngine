#include "JsonStreaming.h"

#include <fstream>
#include <stack>
#include <vector>

#include "Io/Ascii/AsciiReader.h"
#include "Io/Json/ElementType.h"
#include "Io/Json/JsonElement.h"
#include "Io/Json/JsonNumber.h"
#include "Io/Json/JsonString.h"
#include "Logger/Logger.h"

using namespace MyEngine;
using namespace Io::Json;

Json::JsonStreaming::JsonStreaming(const std::wstring& path)
	: m_Stream(path, std::ifstream::binary)
	, m_State(MyEngine::Json::StreamState::Root)
{
}

Json::JsonStreaming::~JsonStreaming()
{
	m_Stream.close();
}

std::string Json::JsonStreaming::GetNextProperty()
{
	if (m_State != StreamState::InObject)
	{
		Logger::PrintError("Not in object");
		return "";
	}
	SkipWhiteSpace(m_Stream);
	char next = m_Stream.get();
	if (next != ',')
		m_Stream.seekg(-1, std::ios_base::cur);
	std::string propertyName{ GetNextString(m_Stream) };
	SkipWhiteSpace(m_Stream);
	next = m_Stream.get();
	if (next != ':')
	{
		Logger::PrintError(": expected but found: " + std::string{ next });
		return "";
	}
	return propertyName;
}

std::string Json::JsonStreaming::GetNextStringValue()
{
	return GetNextString(m_Stream);
}

double Json::JsonStreaming::GetNextNumberValue()
{
	SkipWhiteSpace(m_Stream);
	if (m_Stream.get() != ',')
		m_Stream.seekg(-1, std::ios_base::cur);
	SkipWhiteSpace(m_Stream);
	return JsonNumber::Read(m_Stream);
}

void Json::JsonStreaming::SkipNextScope()
{
	while (!m_Scopes.empty())
		m_Scopes.pop();
	char lastScope{ '\0' };
	SkipWhiteSpace(m_Stream);
	while (true)
	{
		const char next = m_Stream.get();
		if (next == lastScope)
		{
			if (m_Scopes.size() == 1)
				return;
			m_Scopes.pop();
			lastScope = m_Scopes.top();
		}
		else
		{
			if (next == '{')
			{
				m_Scopes.push('}');
				lastScope = '}';
			}
			else if (next == '[')
			{
				m_Scopes.push(']');
				lastScope = ']';
			}
			else if (next == '"')
			{
				m_Scopes.push('"');
				lastScope = '"';
			}
		}
	}
}

void Json::JsonStreaming::GetOutOfScope()
{
	while (!m_Scopes.empty())
		m_Scopes.pop();
	char lastScope{ '\0' };
	while (true)
	{
		const char next = m_Stream.get();
		if (next == lastScope)
		{
			m_Scopes.pop();
			if (m_Scopes.empty())
				lastScope = '\0';
			else
				lastScope = m_Scopes.top();
		}
		else if (next == '{')
		{
			m_Scopes.push('}');
			lastScope = '}';
		}
		else if (next == '[')
		{
			m_Scopes.push(']');
			lastScope = ']';
		}
		else if (next == '"')
		{
			m_Scopes.push('"');
			lastScope = '"';
		}
		else if (next == '}' || next == ']')
		{
			if (!m_Scopes.empty())
				Logger::PrintError("scopes should be empty");
			return;
		}
	}
}

void Json::JsonStreaming::FindProperty(const std::string& propertyName)
{
	while (true)
	{
		const auto nextProp{ GetNextProperty() };
		if (nextProp == propertyName)
			return;
		SkipNextPropertyValue();
	}
}

void Json::JsonStreaming::SkipNextNull()
{
	SkipWhiteSpace(m_Stream);
	if (m_Stream.get() != 'n')
		Logger::PrintError("null not found");
	m_Stream.seekg(3, std::ios_base::cur);
}

void Json::JsonStreaming::SkipNextNumber()
{
	SkipWhiteSpace(m_Stream);
	char next;
	while (m_Stream.get(next))
	{
		if (next != '-' && next != 'f' && next != '.')
			return;
		if (next >= '0' && next <= '9')
			return;
	}
}

void Json::JsonStreaming::SkipNextPropertyValue()
{
	SkipWhiteSpace(m_Stream);
	const auto type{ JsonElement::FindElementType(m_Stream) };
	switch (type)
	{
	case ElementType::Object:
	case ElementType::Array:
	case ElementType::String:
		SkipNextScope();
		break;
	case ElementType::Number: SkipNextNumber(); break;
	case ElementType::Null: SkipNextNull(); break;
	case ElementType::Unknown:
	default:
		Logger::PrintError("trying to skip unknown property-value");;
	}
}

void Json::JsonStreaming::SkipNextProperty()
{
	SkipNextScope();
	SkipWhiteSpace(m_Stream);
	if (m_Stream.get() != ':')
		Logger::PrintError(": expected during skip property");
	SkipNextPropertyValue();
}

bool Json::JsonStreaming::IsEndOfObject()
{
	SkipWhiteSpace(m_Stream);
	const bool isEnd = m_Stream.get() == '}';
	m_Stream.seekg(-1, std::ios_base::cur);
	return isEnd;
}

bool Json::JsonStreaming::IsEndOfArray()
{
	SkipWhiteSpace(m_Stream);
	const bool isEnd = m_Stream.get() == ']';
	m_Stream.seekg(-1, std::ios_base::cur);
	return isEnd;
}

void Json::JsonStreaming::GetInNextObject()
{
	SkipWhiteSpace(m_Stream);
	if (m_Stream.get() == ',')
		SkipWhiteSpace(m_Stream);
	else
		m_Stream.seekg(-1, std::ios_base::cur);

	char next;
	while (m_Stream.get(next))
		if (next == '{')
		{
			m_State = StreamState::InObject;
			return;
		}
	Logger::PrintError("Next object not found");
}

void Json::JsonStreaming::GetInArray()
{
	SkipWhiteSpace(m_Stream);
	if (m_Stream.get() != ',')
		m_Stream.seekg(-1, std::ios_base::cur);
	SkipWhiteSpace(m_Stream);
	if (m_Stream.get() != '[')
		Logger::PrintError("Next element is not an array");
}

ElementType Json::JsonStreaming::FindNextElementType()
{
	return JsonElement::FindElementType(m_Stream);
}

char Json::JsonStreaming::FindNextSign(std::ifstream& stream)
{
	char next;
	while (stream.get(next))
	{
		if (next == '{'
			|| next == '}'
			|| next == '['
			|| next == ']'
			|| next == '"'
			|| next == ':'
			|| next == ',')
		{
			return next;
		}
	}
	return '\0';
}

std::string Json::JsonStreaming::GetNextString(std::ifstream& stream)
{
	std::streampos begin = 0;
	char next;
	while (stream.get(next))
	{
		if (begin == 0)
		{
			if (Io::AsciiReader::IsWhiteSpace(next) || next == ',') continue;
			if (next != '"')
			{
				std::stringstream ss{};
				ss << "[JsonStreaming::GetNextString] "
					<< "unexpected character in get next string: "
					<< next << std::endl;
				Logger::PrintError(ss.str());
				return "";
			}
			begin = stream.tellg();
		}
		else
		{
			if (next != '"') continue;
			const auto size = stream.tellg() - begin;
			stream.seekg(begin);
			char* pChars = new char[size];
			stream.get(pChars, size);
			stream.seekg(1, std::ios_base::cur);
			const std::string value{ pChars };
			delete[] pChars;
			return value;
		}
	}
	Logger::PrintError("Failed finding next property name");
	return "";
}

void Json::JsonStreaming::SkipWhiteSpace(std::ifstream& stream)
{
	char next;
	while (stream.get(next))
		if (!Io::AsciiReader::IsWhiteSpace(next))
		{
			stream.seekg(-1, std::ios_base::cur);
			return;
		}
}
