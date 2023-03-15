#pragma once

namespace MyEngine
{
	namespace Io
	{
		namespace Binary
		{
			class Bini
			{
			public:
				Bini(std::istream& stream);

				unsigned int Uint32();
				uint8_t Uint8();
				std::string String(int size);

				static unsigned int Uint32(std::istream& stream);
				static uint8_t Uint8(std::istream& stream);
				static std::string String(std::istream& stream, int size);
			private:
				std::istream& m_Stream;
			};
		}
	}
}

