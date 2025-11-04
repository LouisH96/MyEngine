#pragma once

#include <Io/Json/JsonElement.h>

namespace MyEngine::Io::Json
{
class JsonBool : public JsonElement
{
public:
	JsonBool(std::ifstream& file);

	std::string ToString() const override;

	bool Get() const { return m_Value; }
	bool GetBool() const { return m_Value; }
	static bool DetectType(char c);
	static bool Read(std::ifstream& stream);

private:
	bool m_Value;
};
}