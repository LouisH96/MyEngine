#pragma once
#include <ostream>

#include "DataStructures/PtrRangeConst.h"

#define CSV_WRITER_DEBUG

namespace MyEngine
{
	class CsvWriter
	{
	public:
		CsvWriter(std::ostream& stream, PtrRangeConst<std::string> columnNames);
		void AddRow(PtrRangeConst<std::string> rowData);

	private:
		std::ostream& m_Stream;
#ifdef CSV_WRITER_DEBUG
		const unsigned m_NrColumns;
#endif
	};
}
