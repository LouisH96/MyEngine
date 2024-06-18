#include "TableDirectory.h"

#include <iostream>

#include "Io/Binary/BigBinReader.h"

void MyEngine::Io::Ttf::TableDirectory::Read(const Bin::BigBinReader& reader)
{
	for (int i = 0; i < TAG_LENGTH; i++)
		m_Tag[i] = reader.Uint8();
	m_Tag[TAG_LENGTH] = '\0';

	reader.Read(m_CheckSum);
	reader.Read(m_Offset);
	reader.Read(m_Length);
}

void MyEngine::Io::Ttf::TableDirectory::Print() const
{
	for (int i = 0; i < TAG_LENGTH; i++)
		std::cout << m_Tag[i];
	std::cout << "\t|" << m_CheckSum << "\t|";
	std::cout << m_Offset << "\t|" << m_Length << std::endl;
}

void MyEngine::Io::Ttf::TableDirectory::PrintHeader()
{
	std::cout << "___| TableDirectory |___\n";
	std::cout << "Tag\t|CheckSum\t|Offset\t|Length\n";
}

std::string MyEngine::Io::Ttf::TableDirectory::GetTag() const
{
	return { reinterpret_cast<const char*>(m_Tag), TAG_LENGTH };
}
