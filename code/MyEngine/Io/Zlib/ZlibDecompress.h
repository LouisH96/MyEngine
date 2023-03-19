#pragma once

namespace MyEngine
{
	namespace Io
	{
		namespace Binary
		{
			class BitStream;
		}
	}
}

namespace MyEngine
{
	namespace Io
	{
		namespace Zlib
		{
			class ZlibDecompress
			{
			public:
				ZlibDecompress() = delete;

				static void Unzip(std::istream& stream);
				static void Deflate(std::istream& stream);

			private:
				static uint16_t ReadLength(uint16_t code, Binary::BitStream& stream);
				static uint16_t ReadDistance(uint8_t code, Binary::BitStream& stream);
				static void PrintLengthTable();
				static void PrintDistanceTable();

			public:
				//tests
				static std::string GetMyTest1Input();
				static std::string GetInternetExampleInput();
				static std::string GetSalsaTestInput();
				static std::string GetLongDutchSentenceTest();
				static bool ValidateInternetExampleOutput(const std::string& output);
				static bool ValidateMyTest1Output(const std::string& output);
			};
		}
	}
}
