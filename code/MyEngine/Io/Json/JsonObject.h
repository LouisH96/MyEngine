#pragma once
#include <map>

#include "Io/Json/JsonElement.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Json
		{
			class JsonObject final : public JsonElement
			{
			public:
				explicit JsonObject(std::ifstream& stream);
				~JsonObject() override;
				JsonObject(const JsonObject& other) = delete;
				JsonObject(JsonObject&& other) noexcept = delete;
				JsonObject& operator=(const JsonObject& other) = delete;
				JsonObject& operator=(JsonObject&& other) noexcept = delete;

				std::string ToString() const override;
				const std::map<std::string, JsonElement*>& GetProperties() const { return m_Properties; }
				static bool DetectType(char c);
			private:
				std::map<std::string, JsonElement*> m_Properties{};

				static bool FindNextProperty(std::ifstream& file, std::string& propertyName);
				static bool FindComma(std::ifstream& stream);
			};
		}
	}
}
