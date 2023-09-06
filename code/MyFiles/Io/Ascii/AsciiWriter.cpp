#include "AsciiWriter.h"

using namespace MyEngine;
using namespace Io::Ascii;

void AsciiWriter::Tabs(std::ostream& stream, int nrTabs)
{
	for (int i = 0; i < nrTabs; i++) stream << '\t';
}

AsciiWriter::AsciiWriter(std::ostream& stream, int nrTabs)
	: m_Stream{ stream }
	, m_NrTabs{ nrTabs }
{
}

void AsciiWriter::SetTabs(unsigned amount)
{
	m_NrTabs = static_cast<int>(amount);
}

void AsciiWriter::IncreaseTab(unsigned amount)
{
	m_NrTabs += static_cast<int>(amount);
}

void AsciiWriter::DecreaseTab(unsigned amount)
{
	m_NrTabs -= static_cast<int>(amount);
}

void AsciiWriter::EndLine()
{
	m_Stream << std::endl;
}
