#pragma once
#include <fstream>

#include "DataStructures/List.h"
#include "Io/Ascii/AsciiReader.h"

namespace MyEngine
{
	class CsvReader
	{
	public:
		explicit CsvReader(const std::wstring& path);

		std::string ReadColumn(const std::string& columnName, unsigned row);
		std::string ReadColumn(unsigned column, unsigned row);
		List<std::string> ReadColumns(const std::string& columnName);
		List<std::string> ReadColumns(unsigned column);
		unsigned GetColumnIdx(const std::string& name);

		template<typename F>
		void ReadColumns(const std::string& columnName, F function);
		template<typename F>
		void ReadColumns(unsigned column, F function);

		List<std::string> GetColumnNames();

		void PrintColumnNames();
		bool IsOpen() const;

	private:
		std::ifstream m_Stream;
	};

	template <typename F>
	void CsvReader::ReadColumns(const std::string& columnName, F function)
	{
		ReadColumns(GetColumnIdx(columnName), function);
	}

	template <typename F>
	void CsvReader::ReadColumns(unsigned column, F function)
	{
		Io::AsciiReader reader{ m_Stream };
		reader.Reset();
		reader.IgnoreLine();

		std::streampos p{ m_Stream.tellg() };
		char next;
		unsigned row{ 0 };
		unsigned currColumn{ 0 };

		while (m_Stream.get(next))
		{
			if (next == ',')
			{
				if (currColumn == column)
				{
					reader.MoveBack();
					function(row, reader.ReadFrom(p));
					reader.IgnoreLine();
					p = reader.GetPos();
					currColumn = 0;
					row++;
					continue;
				}
				p = reader.GetPos();
				currColumn++;
			}
		}
	}
}
