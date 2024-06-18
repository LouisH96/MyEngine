#pragma once
#include <fstream>
#include <vector>

#include "Io/Json/JsonElement.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Json
		{
			class JsonArray final : public JsonElement
			{
			public:
				explicit JsonArray(std::ifstream& stream);
				~JsonArray() override;
				std::string ToString() const override;

				const std::vector<JsonElement*>& GetElements() const { return m_Elements; }
				static bool DetectType(char c);
			private:
				std::vector<JsonElement*> m_Elements{};
			};
		}
	}
}
