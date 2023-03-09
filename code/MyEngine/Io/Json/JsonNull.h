#pragma once
#include "Io/Json/JsonElement.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Json
		{
			class JsonNull final : public JsonElement
			{
			public:
				explicit JsonNull(std::ifstream& file);
				std::string ToString() const override;

				static bool DetectType(char c);
			};
		}
	}
}
