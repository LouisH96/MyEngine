#include "AsciiReader.h"

#include <fstream>
#include "Logger/Logger.h"
#include <String/Convert.h>

#define ASCII_READER_DEBUG

using namespace MyEngine;
using namespace Io;

AsciiReader::ValueType AsciiReader::DetectValueType(std::istream& stream)
{
	const char next = static_cast<char>(stream.peek());

	if (IsNumber(next) || next == '-') return Number;
	if (next == '"') return String;

	return Other;
}

bool AsciiReader::IsNumber(char c)
{
	return c >= '0' && c <= '9';
}

bool AsciiReader::IsWhiteSpace(char c)
{
	return c == ' ' 
		|| c == '\n'
		|| c == '\r'
		|| c == '\t';
}

std::string AsciiReader::ReadFrom(std::istream& stream, const std::streampos& pos)
{
	const std::streampos temp{ stream.tellg() };
	std::string result(stream.tellg() - pos, ' ');
	stream.seekg(pos);
	stream.read(&result[0], static_cast<std::streamsize>(result.size()));  // NOLINT(readability-container-data-pointer)
	return result;
}

std::string AsciiReader::ReadUntil(std::istream& stream, char delim)
{
	const std::streampos begin{ stream.tellg() };

	char next;
	while (stream.get(next))
	{
		if (next == delim)
		{
			MoveBack(stream);
			return ReadFrom(stream, begin);
		}
	}
	Logger::PrintWarning("[AsciiReader::ReadUntil] eof before delim");
	return "";
}

std::string AsciiReader::ReadUntil(std::istream& stream, char delim1, char orDelim2)
{
	const std::streampos begin{ stream.tellg() };

	char next;
	while (stream.get(next))
	{
		if (next == delim1 || next == orDelim2)
		{
			MoveBack(stream);
			return ReadFrom(stream, begin);
		}
	}
	Logger::PrintWarning("[AsciiReader::ReadUntil] eof before delim");
	return "";
}

bool AsciiReader::ReadLine(std::istream& stream, std::string& string)
{
	return !!std::getline(stream, string);
}

std::string AsciiReader::ReadUntil(char delim)
{
	return ReadUntil(m_Stream, delim);
}

std::string AsciiReader::ReadUntil(char delim1, char orDelim2)
{
	return ReadUntil(m_Stream, delim1, orDelim2);
}

bool AsciiReader::ReadLine(std::string& string)
{
	return ReadLine(m_Stream, string);
}

std::string AsciiReader::ReadLine()
{
	std::string line;
	std::getline(m_Stream, line);
	return line;
}

std::string AsciiReader::ReadUntilWhiteSpace()
{
	const std::streampos begin{ m_Stream.tellg() };

	char next;
	while (m_Stream.get(next))
	{
		if (next == ' ' || next == '\t' || next == '\n')
		{
			MoveBack();
			std::string result{ ReadFrom(begin) };
			Move(1);
			return result;
		}
	}
	return ReadFrom(begin);
}

void AsciiReader::Move(std::istream& stream, int amount)
{
	stream.seekg(amount, std::ios_base::cur);
}

void AsciiReader::MoveBack(std::istream& stream, unsigned amount)
{
	stream.seekg(-static_cast<int>(amount), std::ios_base::cur);
}

void AsciiReader::Reset(std::istream& stream)
{
	stream.clear();
	stream.seekg(0);
}

AsciiReader::AsciiReader(const std::wstring& path)
	: m_Stream{ Convert::ToString(path), std::istream::binary }
{
}

AsciiReader::AsciiReader(std::ifstream&& stream)
	: m_Stream{ std::move(stream) }
{
}

bool AsciiReader::GetChar(char& c)
{
	m_Stream.get(c);
	return !m_Stream.eof();
}

char AsciiReader::GetChar()
{
	return m_Stream.get();
}

char AsciiReader::PeekChar()
{
	return static_cast<char>(m_Stream.peek());
}

std::streampos AsciiReader::GetPos()
{
	return m_Stream.tellg();
}

void AsciiReader::MoveBack(unsigned amount)
{
	MoveBack(m_Stream, amount);
}

void AsciiReader::MoveTo(std::streampos pos)
{
	m_Stream.seekg(pos);
}

void AsciiReader::Reset()
{
	Reset(m_Stream);
}

void AsciiReader::Ignore(std::istream& stream, unsigned amount)
{
	stream.ignore(amount);
}

void AsciiReader::Ignore(std::istream& stream, char c, unsigned amount)
{
	while (amount > 0)
	{
		IgnoreUntil(stream, c);
		amount--;
	}
}

void AsciiReader::IgnoreUntil(std::istream& stream, char c)
{
	stream.ignore(std::numeric_limits<std::streamsize>::max(), c);
}

void AsciiReader::IgnoreLine(std::istream& stream)
{
	IgnoreUntil(stream, '\n');
}

void AsciiReader::IgnoreLines(std::istream& stream, unsigned amount)
{
	while (amount > 0)
	{
		IgnoreLine(stream);
		amount--;
	}
}

void AsciiReader::Ignore(unsigned amount)
{
	m_Stream.ignore(amount);
}

void AsciiReader::Ignore(char c, unsigned amount)
{
	Ignore(m_Stream, c, amount);
}

void AsciiReader::IgnoreLine()
{
	IgnoreLine(m_Stream);
}

void AsciiReader::IgnoreLines(unsigned amount)
{
	IgnoreLines(m_Stream, amount);
}

void AsciiReader::IgnoreUntil(char c)
{
	IgnoreUntil(m_Stream, c);
}

std::string AsciiReader::GetUntil(char delim)
{
	std::string result{};
	std::getline(m_Stream, result, delim);
	return result;
}

double AsciiReader::GetDouble(char separator)
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

int AsciiReader::GetInteger()
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

std::string AsciiReader::GetString()
{
#ifdef ASCII_READER_DEBUG
	if (GetChar() != '"')
		Logger::PrintError("[AsciiReader::GetString] first character should be \"");
#else
	GetChar();
#endif
	return GetUntil('"');
}

bool AsciiReader::Good()
{
	return m_Stream.good();
}

bool AsciiReader::IsOpen()
{
	return m_Stream.is_open();
}
