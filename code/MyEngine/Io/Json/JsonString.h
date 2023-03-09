#pragma once
#include "Io/Json/JsonElement.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Json
		{
			class JsonString final : public JsonElement
			{
			public:
				explicit JsonString(std::ifstream& stream);
				std::string ToString() const override;

				const std::string& Get() const { return m_Value; }
				static bool DetectType(char c);
			private:
				std::string m_Value;
			};
		}
	}
}
