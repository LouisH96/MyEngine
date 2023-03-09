#pragma once
#include "ElementType.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Json
		{
			class JsonObject;
			class JsonArray;
			class JsonString;
			class JsonNumber;

			class JsonElement
			{
			public:
				JsonElement() = default;
				virtual ~JsonElement() = default;

				virtual std::string ToString() const = 0;

				const JsonObject& AsObject() const;
				const JsonArray& AsArray() const;
				const JsonString& AsString() const;
				const JsonNumber& AsNumber() const;

				static ElementType FindElementType(std::ifstream& stream);
				static JsonElement* ReadElement(std::ifstream& stream);
			};
		}
	}
}
