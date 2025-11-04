#include "CsvReader.h"

#include <string>

#include "Io/Ascii/AsciiReader.h"
#include "Logger/Logger.h"

using namespace MyEngine;
using namespace Io;

CsvReader::CsvReader(const std::wstring& path)
	: m_Reader{ path }
{
}

std::string CsvReader::ReadColumn(const std::string& columnName, unsigned row)
{
	return ReadColumn(GetColumnIdx(columnName), row);
}

std::string CsvReader::ReadColumn(unsigned column, unsigned row)
{
	m_Reader.Reset();
	m_Reader.IgnoreLines(row + 1);
	m_Reader.Ignore(',', column);
	return m_Reader.ReadUntil(',', '\n');
}

List<std::string> CsvReader::ReadColumns(const std::string& columnName)
{
	return ReadColumns(GetColumnIdx(columnName));
}

List<std::string> CsvReader::ReadColumns(unsigned column)
{
	List<std::string> values{};
	m_Reader.Reset();
	m_Reader.IgnoreLine();

	while (m_Reader.Good())
	{
		m_Reader.Ignore(',', column);
		values.Add(m_Reader.ReadUntil(',', '\n'));
		m_Reader.IgnoreLine();
		while (m_Reader.PeekChar() == '\n')
			m_Reader.Ignore(1);
	}
	return values;
}

unsigned CsvReader::GetColumnIdx(const std::string& name)
{
	m_Reader.Reset();

	std::streampos begin{ m_Reader.GetPos() };
	char next;
	unsigned idx{ 0 };

	while (m_Reader.GetChar(next))
	{
		if (next == ',')
		{
			m_Reader.MoveBack();
			const std::string currentColumn{ m_Reader.ReadFrom(begin) };
			if (currentColumn == name)
				return idx;
			idx++;
			m_Reader.Ignore(1);
			begin = m_Reader.GetPos();
		}
	}
	Logger::Warning("[CsvReader::GetColumnIdx] ColumnName not found");
	return static_cast<unsigned>(-1);
}

List<std::string> CsvReader::GetColumnNames()
{
	List<std::string> names{};

	m_Reader.MoveTo(0);
	std::streampos begin{ m_Reader.GetPos() };
	char next;

	while (m_Reader.GetChar(next))
	{
		if (next == ',')
		{
			m_Reader.MoveBack(1);
			names.Add(m_Reader.ReadFrom(begin));
			m_Reader.Ignore(1);
			begin = m_Reader.GetPos();
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

bool CsvReader::IsOpen()
{
	return m_Reader.IsOpen();
}
