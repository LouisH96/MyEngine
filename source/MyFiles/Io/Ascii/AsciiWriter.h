#pragma once
#include <ostream>

namespace MyEngine
{
	namespace Io
	{
		namespace Ascii
		{
			class AsciiWriter
			{
			public:
				static void Tabs(std::ostream& stream, int nrTabs);

				explicit AsciiWriter(std::ostream& stream, int nrTabs = 0);

				void SetTabs(unsigned amount);
				void IncreaseTab(unsigned amount = 1);
				void DecreaseTab(unsigned amount = 1);
				void EndLine();

				template<typename T>
				std::ostream& operator<<(T value);

			private:
				std::ostream& m_Stream;
				int m_NrTabs;
			};

			template <typename T>
			std::ostream& AsciiWriter::operator<<(T value)
			{
				Tabs(m_Stream, m_NrTabs);
				return m_Stream << value;
			}
		}
	}
}
