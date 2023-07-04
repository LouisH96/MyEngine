#include "AsciiWriter.h"

void MyEngine::Io::Ascii::AsciiWriter::Tabs(std::ostream& stream, int nrTabs)
{
	for (int i = 0; i < nrTabs; i++) stream << '\t';
}

MyEngine::Io::Ascii::AsciiWriter::AsciiWriter(std::ostream& stream, int nrTabs)
	: m_Stream{ stream }
	, m_NrTabs{ nrTabs }
{
}
