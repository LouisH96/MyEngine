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
				static std::vector<uint8_t> Decompress(std::istream& stream);

			private:
				explicit DeflateDecompress(std::istream& stream);
				std::istream& m_Input;
				BitStream m_BitStream;
				std::vector<uint8_t> m_Output{};

				//---| Fixed Huffman codes (BTYPE=01) |---
				void HandleFixedBlock();
				void HandleLengthDistancePair(uint16_t lengthCode);
				uint16_t ReadLength(uint16_t code, bool flipped = false);
				uint16_t ReadDistance(uint8_t code, bool flipped = false);
				static void PrintFixedLengthTable();
				static void PrintFixedDistanceTable();

				//---| Dynamic Huffman codes (BTYPE=10) |---
				void HandleDynamicBlock();
				void ReadClCodeLengths(uint8_t hcLen, Array<uint8_t>& lengths);
				void GetCodeLengths(Array<uint8_t>& output, const Array<uint16_t>& clSymbols,const Array<uint8_t>& clSymbolsLengths, uint16_t amount);
				uint8_t GetNextClSymbol(const Array<uint16_t>& clSymbolCodes, const Array<uint8_t>& clSymbolCodeLengths);
				static void CopyToOutput(Array<uint8_t>& output, uint32_t startIdx, uint8_t value, uint8_t amount);
				static void CopyToOutput(Array<uint16_t>& output, uint32_t startIdx, uint16_t value, uint8_t amount);
				void ReadDynamicBlockData(const Array<uint16_t>& llCodes, const Array<uint8_t>& llCodeLengths, const Array<uint16_t>& dCodes, const Array<uint8_t>& dCodeLengths);
				uint16_t GetNextCl(const Array<uint16_t>& codes, const Array<uint8_t>& codeLengths);

			public:
				//tests
				static std::string GetMyTest1Input();
				static std::string GetInternetExampleInput();
				static std::string GetSalsaTestInput();
				static std::string GetLongDutchSentenceTest();
				static std::string GetLongDutchSentence2Test();
				static std::string GetBlueTest();
				
				static bool ValidateInternetExampleOutput(const std::string& output);
				static bool ValidateMyTest1Output(const std::string& output);
			};
		}
	}
}
