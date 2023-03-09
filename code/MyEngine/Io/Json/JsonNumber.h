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

				double Get() const { return m_Number; }
				float GetFloat() const { return static_cast<float>(m_Number); }
				static bool DetectType(char c);
			private:
				double m_Number;
			};
		}
	}
}
