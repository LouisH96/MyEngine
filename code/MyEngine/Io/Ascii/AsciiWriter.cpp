#include "pch.h"
#include "AsciiWriter.h"

void Io::Ascii::AsciiWriter::Tabs(std::ostream& stream, int nrTabs)
{
	for (int i = 0; i < nrTabs; i++) stream << '\t';
}

Io::Ascii::AsciiWriter::AsciiWriter(std::ostream& stream, int nrTabs)
	: m_Stream{ stream }
	, m_NrTabs{ nrTabs }
{
}
