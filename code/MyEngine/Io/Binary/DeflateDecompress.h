#pragma once
#include <vector>

#include "BitStream.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Binary
		{
			class BitStream;

			class DeflateDecompress
			{
			public:
				explicit DeflateDecompress(std::istream& stream);

			private:
				std::istream& m_Input;
				BitStream m_BitStream;
				std::vector<uint8_t> m_Output;

				//---| Fixed Huffman codes (BTYPE=01) |---
				void HandleFixedBlock();
				void HandleLengthDistancePair(uint16_t lengthCode);
				uint16_t ReadLength(uint16_t code);
				uint16_t ReadDistance(uint8_t code);
				static void PrintFixedLengthTable();
				static void PrintFixedDistanceTable();

				//---| Dynamic Huffman codes (BTYPE=10) |---
				void HandleDynamicBlock();

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
