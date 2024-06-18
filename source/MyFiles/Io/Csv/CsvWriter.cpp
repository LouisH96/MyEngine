#include "CsvWriter.h"

using namespace MyEngine;

CsvWriter::CsvWriter(std::ostream& stream, PtrRangeConst<std::string> columnNames)
	: m_Stream{ stream }
#ifdef CSV_WRITER_DEBUG
	, m_NrColumns{ columnNames.count }
#endif
{
	for (unsigned i = 0; i + 1 < columnNames.count; i++)
		m_Stream << columnNames[i] << ",";
	m_Stream << columnNames.Last() << std::endl;
}

void CsvWriter::AddRow(PtrRangeConst<std::string> rowData)
{
	for (unsigned i = 0; i + 1 < rowData.count; i++)
		m_Stream << rowData[i] << ",";
	m_Stream << rowData.Last() << std::endl;
}
