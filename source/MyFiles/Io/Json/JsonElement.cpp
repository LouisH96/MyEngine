#include "JsonElement.h"

#include <fstream>

#include "JsonArray.h"
#include "JsonNull.h"
#include "JsonNumber.h"
#include "JsonObject.h"
#include "JsonString.h"
#include "Logger/Logger.h"

const MyEngine::Io::Json::JsonObject& MyEngine::Io::Json::JsonElement::AsObject() const
{
	return *reinterpret_cast<const JsonObject*>(this);
}

const MyEngine::Io::Json::JsonArray& MyEngine::Io::Json::JsonElement::AsArray() const
{
	return *reinterpret_cast<const JsonArray*>(this);
}

const MyEngine::Io::Json::JsonString& MyEngine::Io::Json::JsonElement::AsString() const
{
	return *reinterpret_cast<const JsonString*>(this);
}

const MyEngine::Io::Json::JsonNumber& MyEngine::Io::Json::JsonElement::AsNumber() const
{
	return *reinterpret_cast<const JsonNumber*>(this);
}

MyEngine::Io::Json::ElementType MyEngine::Io::Json::JsonElement::FindElementType(std::ifstream& stream)
{
	char c;
	while (stream.get(c))
	{
		if (c == ' ' || c == '\n') continue;
		stream.seekg(-1, std::ios_base::cur);
		if (JsonArray::DetectType(c))  return ElementType::Array;
		if (JsonObject::DetectType(c)) return ElementType::Object;
		if (JsonString::DetectType(c)) return ElementType::String;
		if (JsonNumber::DetectType(c)) return ElementType::Number;
		if (JsonNull::DetectType(c)) return ElementType::Null;
		Logger::PrintError("Unknown character");
		return ElementType::Unknown;
	}
	return ElementType::Unknown;
}

MyEngine::Io::Json::JsonElement* MyEngine::Io::Json::JsonElement::ReadElement(std::ifstream& stream)
{
	switch (FindElementType(stream))
	{
	case ElementType::String: return new JsonString(stream);
	case ElementType::Object: return new JsonObject(stream);
	case ElementType::Array: return new JsonArray(stream);
	case ElementType::Number: return new JsonNumber(stream);
	case ElementType::Null: return new JsonNull(stream);
	case ElementType::Unknown:
	default:
		Logger::PrintError("Unknown ElementType");
		return nullptr;
	}
}

std::string MyEngine::Io::Json::JsonElement::TypeToString(ElementType type)
{
	switch(type)
	{
	case ElementType::Object: return "Object";
	case ElementType::Array: return "Array";
	case ElementType::String: return "String";
	case ElementType::Number: return "Number";
	case ElementType::Null: return "Null";
	case ElementType::Unknown: return "Unknown";
	default: 
		Logger::PrintError("Unknown ElementType");
		return "";
	}
}
