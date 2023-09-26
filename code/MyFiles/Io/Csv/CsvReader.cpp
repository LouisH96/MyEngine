#include "CsvReader.h"

#include <string>

#include "Io/Ascii/AsciiReader.h"
#include "Logger/Logger.h"

using namespace MyEngine;
using namespace Io;

CsvReader::CsvReader(const std::wstring& path)
	: m_Stream{ path, std::istream::binary }
{
	if (!m_Stream.is_open())
	{
		Logger::PrintError("[CsvReader] stream isn't open");
	}
}

std::string CsvReader::ReadColumn(const std::string& columnName, unsigned row)
{
	return ReadColumn(GetColumnIdx(columnName), row);
}

std::string CsvReader::ReadColumn(unsigned column, unsigned row)
{
	AsciiReader reader{ m_Stream };
	reader.IgnoreLines(row + 1);
	reader.Ignore(',', column);
	return reader.ReadUntil(',', '\n');
}

List<std::string> CsvReader::ReadColumns(const std::string& columnName)
{
	return ReadColumns(GetColumnIdx(columnName));
}

List<std::string> CsvReader::ReadColumns(unsigned column)
{
	List<std::string> values{};
	AsciiReader reader{ m_Stream };
	reader.Reset();
	reader.IgnoreLine();

	while (m_Stream.good())
	{
		reader.Ignore(',', column);
		values.Add(reader.ReadUntil(',', '\n'));
		reader.IgnoreLine();
		while (reader.PeekChar() == '\n')
			reader.Ignore(1);
		std::cout << values[values.GetSizeS() - 1] << std::endl;
	}
	return values;
}

unsigned CsvReader::GetColumnIdx(const std::string& name)
{
	AsciiReader::Reset(m_Stream);

	std::streampos begin{ m_Stream.tellg() };
	char next;
	unsigned idx{ 0 };

	while (m_Stream.get(next))
	{
		if (next == ',')
		{
			AsciiReader::MoveBack(m_Stream);
			const std::string currentColumn{ AsciiReader::ReadFrom(m_Stream, begin) };
			if (currentColumn == name)
				return idx;
			idx++;
			AsciiReader::Ignore(m_Stream, 1);
			begin = m_Stream.tellg();
		}
	}
	Logger::PrintWarning("[CsvReader::GetColumnIdx] ColumnName not found");
	return static_cast<unsigned>(-1);
}

List<std::string> CsvReader::GetColumnNames()
{
	List<std::string> names{};

	m_Stream.seekg(0);
	std::streampos begin{ m_Stream.tellg() };
	char next;

	while (m_Stream.get(next))
	{
		if (next == ',')
		{
			m_Stream.seekg(-1, std::istream::cur);
			names.Add(Io::AsciiReader::ReadFrom(m_Stream, begin));
			m_Stream.ignore();
			begin = m_Stream.tellg();
		}
		else if (next == '\n')
		{
			return names;
		}
	}
	return names;
}

void CsvReader::PrintColumnNames()
{
	List<std::string> names{ GetColumnNames() };
	for (unsigned i = 0; i < names.GetSize(); i++)
		std::cout << names[i] << std::endl;
}

bool CsvReader::IsOpen() const
{
	return m_Stream.is_open();
}
