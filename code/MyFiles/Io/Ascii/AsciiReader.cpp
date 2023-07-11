#include "AsciiReader.h"

#include <istream>

#include "Logger/Logger.h"

#define ASCII_READER_DEBUG

MyEngine::Io::AsciiReader::ValueType MyEngine::Io::AsciiReader::DetectValueType(std::istream& stream)
{
	const char next = static_cast<char>(stream.peek());

	if (IsNumber(next) || next == '-') return Number;
	if (next == '"') return String;

	return Other;
}

bool MyEngine::Io::AsciiReader::IsNumber(char c)
{
	return c >= '0' && c <= '9';
}

std::string MyEngine::Io::AsciiReader::ReadFrom(std::istream& stream, const std::streampos& pos)
{
	std::string result(stream.tellg() - pos, ' ');
	stream.read(&result[0], result.size());  // NOLINT(readability-container-data-pointer)
	return result;
}

void MyEngine::Io::AsciiReader::Move(std::istream& stream, int amount)
{
	stream.seekg(amount, std::ios_base::cur);
}

MyEngine::Io::AsciiReader::AsciiReader(std::istream& stream)
	: m_Stream{ stream }
{
}

bool MyEngine::Io::AsciiReader::GetChar(char& c)
{
	m_Stream.get(c);
	return !m_Stream.eof();
}

char MyEngine::Io::AsciiReader::GetChar()
{
	return m_Stream.get();
}

char MyEngine::Io::AsciiReader::PeekChar()
{
	return static_cast<char>(m_Stream.peek());
}

std::streampos MyEngine::Io::AsciiReader::GetPos() const
{
	return m_Stream.tellg();
}

void MyEngine::Io::AsciiReader::Ignore(unsigned amount)
{
	m_Stream.ignore(amount);
}

void MyEngine::Io::AsciiReader::IgnoreLine()
{
	IgnoreUntil('\n');
}

void MyEngine::Io::AsciiReader::IgnoreUntil(char c)
{
	m_Stream.ignore(std::numeric_limits<std::streamsize>::max(), c);
}

std::string MyEngine::Io::AsciiReader::GetUntil(char delim)
{
	std::string result{};
	std::getline(m_Stream, result, delim);
	return result;
}

double MyEngine::Io::AsciiReader::GetDouble(char separator)
{
	//negative?
	double isNegative;
	if (GetChar() == '-')
		isNegative = -1;
	else
	{
		isNegative = 1;
		Move(-1);
	}

	double number{ 0 };
	double decimalScale{ 10 };

	//integer
	char next;
	while (GetChar(next))
	{
		if (!IsNumber(next))
		{
			if (next == separator)
			{
				//decimal
				while (GetChar(next))
				{
					if (next == 'e' || next == 'E')
					{
						//exponent
						const int exponent{ GetInteger() };
						return isNegative * number * pow(10, exponent);
					}
					if (!IsNumber(next))
						break;
					number += (next - '0') / decimalScale;
					decimalScale *= 10;
				}
			}
			Move(-1);
			break;
		}
		if (next == 'e' || next == 'E')
			Logger::PrintError("[AsciiReader::GetDouble] e found before separator");
		number *= 10;
		number += next - '0';
	}
	return number * isNegative;
}

int MyEngine::Io::AsciiReader::GetInteger()
{
	//negative?
	int isNegative;
	if (GetChar() == '-')
		isNegative = -1;
	else
	{
		isNegative = 1;
		Move(-1);
	}

	int number{ 0 };

	char next;
	while (GetChar(next))
	{
		if (!IsNumber(next))
		{
			Move(-1);
			break;
		}
		number *= 10;
		number += next - '0';
	}
	return number * isNegative;
}

std::string MyEngine::Io::AsciiReader::GetString()
{
#ifdef ASCII_READER_DEBUG
	if (GetChar() != '"')
		Logger::PrintError("[AsciiReader::GetString] first character should be \"");
#else
	GetChar();
#endif
	return GetUntil('"');
}
