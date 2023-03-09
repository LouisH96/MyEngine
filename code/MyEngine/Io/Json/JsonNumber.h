#pragma once
#include "Io/Json/JsonElement.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Json
		{
			class JsonNumber : public JsonElement
			{
			public:
				JsonNumber(std::ifstream& file);
				std::string ToString() const override;

				float Get() const { return m_Number; }
				static bool DetectType(char c);
			private:
				float m_Number;
			};
		}
	}
}
