#include "pch.h"
#include "DeflateDecompress.h"

#include <vector>
#include <Io/Binary/Huffman.h>

#include "BitStream.h"

//use 'backreference' word instead of lengthDistancePair

using namespace Io::Binary;

DeflateDecompress::DeflateDecompress(std::istream& stream)
	: m_Input(stream)
	, m_BitStream(stream)
	, m_Output()
{
	const uint8_t isEnd = m_BitStream.ReadBits(1);
	if (!isEnd)
		Logger::PrintError("Multiple blocks not supported yet");
	const uint8_t bType = m_BitStream.ReadBits(1) + (m_BitStream.ReadBits(1) << 1);

	switch (bType)
	{
	case 1: HandleFixedBlock(); break;
	case 2: HandleDynamicBlock(); break;
	case 0: Logger::PrintError("Block has no compression, which is not supported yet"); return;
	case 3: Logger::PrintError("Block-type was 3 which shouldn't be used"); return;
	default: Logger::PrintError("Unknown block-type (BTYPE) of " + std::to_string(bType)); return;
	}

	for (int i = 0; i < m_Output.size(); i++)
		std::cout << m_Output[i];
	//std::cout << "0x" << std::hex << static_cast<int>(m_Output[i]) << std::endl;
}


void DeflateDecompress::HandleFixedBlock()
{
	while (true)
	{
		const uint8_t next7 = m_BitStream.ReadBits(7);
		if (next7 <= 0b001'0111)
		{
			//end of block
			if (next7 == 0)
				break;
			//length-distance
			HandleLengthDistancePair(next7 + 256);
			continue;
		}
		const uint8_t next8 = next7 << 1 | m_BitStream.ReadBits(1);
		if (next8 <= 0b1011'1111)
		{
			//literal
			m_Output.push_back(next8 - 0b0011'0000);
			continue;
		}
		if (next8 <= 0b1100'0111)
		{
			//length-distance
			HandleLengthDistancePair(next8 + 256);
			continue;
		}
		//literal
		const uint16_t next9 = next8 << 1 | m_BitStream.ReadBits(1);
		m_Output.push_back(next9 - 0b1'1001'0000 + 144);
	}
}

void DeflateDecompress::HandleLengthDistancePair(uint16_t lengthCode)
{
	const uint16_t length = ReadLength(lengthCode);
	const uint8_t distanceCode = m_BitStream.ReadBits(5);
	const uint16_t distance = ReadDistance(distanceCode);

	std::vector<uint8_t> newChars{};
	const int begin = m_Output.size() - distance;
	const int end = begin + length;
	for (int i = begin; i < end; i++)
	{
		const int outputIdx = begin + ((i - begin) % (m_Output.size() - begin));
		newChars.push_back(m_Output[outputIdx]);
	}
	for (int i = 0; i < newChars.size(); i++)
		m_Output.push_back(newChars[i]);
}

uint16_t DeflateDecompress::ReadLength(uint16_t code, bool flipped)
{
	if (code <= 264)
		return code - 257 + 3;
	if (code == 285)
		return 258;
	const uint8_t base = code - 261;
	const uint8_t nrExtraBits = base / 4;
	const uint8_t modulo = base % 4;
	const uint8_t extraBitsValue = flipped ? m_BitStream.ReadBitsFlipped(nrExtraBits) : m_BitStream.ReadBits(nrExtraBits);
	uint16_t length = (1 << nrExtraBits) - 1 << 2;
	length += (1 << nrExtraBits) * modulo;
	length += 7;
	length += extraBitsValue;
	return length;
}

uint16_t DeflateDecompress::ReadDistance(uint8_t code, bool flipped)
{
	if (code <= 3)
		return code + 1;
	const uint8_t base = code - 2;
	const uint8_t nrExtraBits = base / 2;
	const uint8_t modulo = base % 2;
	const uint16_t extraBitsValue = flipped ? m_BitStream.ReadBitsFlipped<uint16_t>(nrExtraBits) :  m_BitStream.ReadBits<uint16_t>(nrExtraBits);
	uint16_t distance = (1 << nrExtraBits - 1) - 1 << 2;
	distance += modulo << nrExtraBits;
	distance += 5;
	distance += extraBitsValue;
	return distance;
}

void DeflateDecompress::HandleDynamicBlock()
{
	const uint8_t hLit{ m_BitStream.ReadBitsFlipped(5) };
	const uint8_t hDist{ m_BitStream.ReadBitsFlipped(5) };
	const uint8_t hClen{ m_BitStream.ReadBitsFlipped(4) };

	Array<uint8_t> clSymbolCodeLengths{ 19 };
	ReadClCodeLengths(hClen, clSymbolCodeLengths);
	Array<uint8_t> clSymbolCodes = Huffman<uint8_t>::GetCodesFromCodeLengths(clSymbolCodeLengths);

	Array<uint8_t> llCodeLengths{ 286, 0 };
	GetCodeLengths(llCodeLengths, clSymbolCodes, clSymbolCodeLengths, hLit + 257);
	Array<uint8_t> dCodeLengths{ 32,0 };
	GetCodeLengths(dCodeLengths, clSymbolCodes, clSymbolCodeLengths, hDist + 1);

	const Array<uint8_t> llCodes = Huffman<uint8_t>::GetCodesFromCodeLengths(llCodeLengths);
	const Array<uint8_t> dCodes = Huffman<uint8_t>::GetCodesFromCodeLengths(dCodeLengths);

	ReadDynamicBlockData(llCodes, llCodeLengths, dCodes, dCodeLengths);
}

void DeflateDecompress::ReadClCodeLengths(uint8_t hcLen, Array<uint8_t>& lengths)
{
	constexpr uint8_t nrBits = 3;
	for (uint8_t i = 16; i <= 18; i++)
		lengths[i] = m_BitStream.ReadBitsFlipped(nrBits);
	lengths[0] = m_BitStream.ReadBitsFlipped(nrBits);

	for (uint8_t i = 0; i < hcLen; i++)
	{
		const uint8_t iPair = i / 2;
		uint8_t iLengths;
		if (i % 2 == 0)
			iLengths = 8 + iPair;
		else
			iLengths = 7 - iPair;
		lengths[iLengths] = m_BitStream.ReadBitsFlipped(nrBits);
	}
	for (uint8_t i = hcLen; i <= 18 - 4; i++)
	{
		const uint8_t iPair = i / 2;
		uint8_t iLengths;
		if (i % 2 == 0)
			iLengths = 8 + iPair;
		else
			iLengths = 7 - iPair;
		lengths[iLengths] = 0;
	}
}

void DeflateDecompress::GetCodeLengths(Array<uint8_t>& output, const Array<uint8_t>& clSymbols, const Array<uint8_t>& clSymbolsLengths,
	uint16_t amount)
{
	for (int iCodeLength = 0; iCodeLength < amount; iCodeLength++)
	{
		const uint8_t symbolCode = GetNextClSymbol(clSymbols, clSymbolsLengths);
		if (symbolCode <= 15)
		{
			output[iCodeLength] = symbolCode;
			continue;
		}
		if (symbolCode == 16)
		{
			const uint8_t nrCopies = 3 + m_BitStream.ReadBitsFlipped(2);
			const uint8_t valueToCopy = output[iCodeLength - 1];
			CopyToOutput(output, iCodeLength, valueToCopy, nrCopies);
			iCodeLength += nrCopies - 1;
			continue;
		}
		if (symbolCode == 17)
		{
			const uint8_t nrCopies = 3 + m_BitStream.ReadBitsFlipped(3);
			CopyToOutput(output, iCodeLength, 0, nrCopies);
			iCodeLength += nrCopies - 1;
			continue;
		}
		if (symbolCode == 18)
		{
			const uint8_t nrCopies = 11 + m_BitStream.ReadBitsFlipped(7);
			CopyToOutput(output, iCodeLength, 0, nrCopies);
			iCodeLength += nrCopies - 1;
			continue;;
		}
		Logger::PrintError("Unknown codeLengthSymbolCode found");
		return;
	}
}

uint8_t DeflateDecompress::GetNextClSymbol(const Array<uint8_t>& clSymbolCodes,
	const Array<uint8_t>& clSymbolCodeLengths)
{
	uint8_t previousLength = 0;
	uint8_t readValue = 0;

	while (true)
	{
		//find shortest
		uint8_t shortest = static_cast<uint8_t>(255);
		for (int i = 0; i < clSymbolCodeLengths.GetSize(); i++)
			if (clSymbolCodeLengths[i] > previousLength && clSymbolCodeLengths[i] < shortest)
				shortest = clSymbolCodeLengths[i];
		if (shortest == 255)
		{
			Logger::PrintError("No shorter code found");
			return 0;
		}

		//try find valid clSymbolCode)
		readValue <<= shortest - previousLength;
		readValue |= m_BitStream.ReadBits(shortest - previousLength);

		for (int i = 0; i < clSymbolCodeLengths.GetSize(); i++)
		{
			if (clSymbolCodeLengths[i] == shortest &&
				clSymbolCodes[i] == readValue)
				return i;
		}
		previousLength = shortest;
	}
}

void DeflateDecompress::CopyToOutput(Array<uint8_t>& output, uint8_t startIdx, uint8_t value, uint8_t amount)
{
	for (int i = startIdx; i < startIdx + amount; i++)
		output[i] = value;
}

void DeflateDecompress::ReadDynamicBlockData(const Array<uint8_t>& llCodes, const Array<uint8_t>& llCodeLengths,
	const Array<uint8_t>& dCodes, const Array<uint8_t>& dCodeLengths)
{
	while (true)
	{
		const uint16_t next = GetNextCl(llCodes, llCodeLengths);
		if(next <=255)
		{
			m_Output.push_back(next);
			continue;
		}
		if (next ==256)
			return;
		if(next <= 285)
		{
			const uint16_t length = ReadLength(next, true);
			const uint8_t distanceCode = GetNextCl(dCodes, dCodeLengths);
			const uint16_t distance = ReadDistance(distanceCode, true);

			std::vector<uint8_t> newChars{};
			const int begin = m_Output.size() - distance;
			const int end = begin + length;
			for (int i = begin; i < end; i++)
			{
				const int outputIdx = begin + ((i - begin) % (m_Output.size() - begin));
				newChars.push_back(m_Output[outputIdx]);
			}
			for (int i = 0; i < newChars.size(); i++)
				m_Output.push_back(newChars[i]);
			continue;
		}
		Logger::PrintError("Unknown code");
		return;
	}
}

uint16_t DeflateDecompress::GetNextCl(const Array<uint8_t>& allCodes, const Array<uint8_t>& allCodeLengths)
{
	uint8_t previousLength = 0;
	uint16_t readValue = 0;

	while (true)
	{
		//find shortest
		uint8_t shortest = static_cast<uint8_t>(255);
		for (int i = 0; i < allCodeLengths.GetSize(); i++)
			if (allCodeLengths[i] > previousLength && allCodeLengths[i] < shortest)
				shortest = allCodeLengths[i];
		if (shortest == 255)
		{
			Logger::PrintError("No shorter code found");
			return 0;
		}

		//try find valid clSymbolCode)
		readValue <<= shortest - previousLength;
		readValue |= m_BitStream.ReadBits(shortest - previousLength);

		for (int i = 0; i < allCodeLengths.GetSize(); i++)
		{
			if (allCodeLengths[i] == shortest &&
				allCodes[i] == readValue)
				return i;
		}
		previousLength = shortest;
	}
}

void DeflateDecompress::PrintFixedLengthTable()
{
	std::cout << "---| Length Table |---\n";
	for (int i = 257; i <= 285; i++)
	{
		if (i <= 264)
		{
			std::cout << i << "\t|0|\t|" << i - 257 + 3 << std::endl;
			continue;
		}
		if (i == 285)
		{
			std::cout << i << "\t|0|\t|285" << std::endl;
			continue;
		}
		const uint8_t base = i - 261;
		const uint8_t divide = base / 4;
		const uint8_t modulo = base % 4;

		const uint16_t groupBase = (1 << divide) - 1 << 2;
		const uint16_t addition = (1 << divide) * modulo;
		const uint16_t beginRange = groupBase + addition + 7;
		const uint16_t endRange = beginRange + ((1 << divide) - 1);

		std::cout << i << "\t|" << std::to_string(divide) << "\t|" << beginRange << "-" << endRange << std::endl;
	}
}

void DeflateDecompress::PrintFixedDistanceTable()
{
	std::cout << "---| Distance Table |---\n";
	for (int i = 0; i <= 29; i++)
	{
		if (i <= 3)
		{
			std::cout << i << "\t|0\t|" << i + 1 << std::endl;
			continue;
		}
		const uint8_t base = i - 2;
		const uint8_t nrBits = base / 2;
		const uint8_t modulo = base % 2;

		const uint16_t divideBase = ((1 << nrBits - 1) - 1) << 2;
		const uint16_t moduloAdd = modulo << nrBits;
		const uint16_t beginRange = divideBase + moduloAdd + 5;
		const uint16_t endRange = beginRange + ((1 << nrBits) - 1);

		std::cout << i << "\t|" << std::to_string(nrBits) << "\t|" << beginRange << "-" << endRange << std::endl;
	}
}

std::string DeflateDecompress::GetMyTest1Input()
{
	constexpr uint8_t input[]
	{
		0x78, 0x9c, 0xf3, 0xc9, 0x2f, 0xcd, 0x2c, 0x56, 0xf0, 0x48, 0x2d,
		0xca, 0x4d, 0xcc, 0x03, 0x20, 0x1d, 0x04, 0x04 ,0x88
	};
	constexpr int inputSize = sizeof(input);
	std::string string(inputSize, '\0');
	for (int i = 0; i < inputSize; i++)
		string[i] = input[i];
	return string;
}

std::string DeflateDecompress::GetInternetExampleInput()
{
	constexpr uint8_t input[]
	{
		0x78, 0x01, 0x63, 0x60, 0x00, 0x01, 0xCD, 0x03, 0x60, 0x0A, 0x4E,
		0x43, 0x78, 0x40, 0x71, 0x07, 0x1C, 0xE2, 0x07, 0xA0, 0xE2, 0x50,
		0x79, 0x08, 0x0F, 0x49, 0x3D, 0xBA, 0xF8, 0x01, 0x88, 0x0A, 0x98,
		0x3D, 0x96, 0x50, 0x79, 0x74, 0xF3, 0xE1, 0xE2, 0x30, 0xF5, 0x50,
		0x75, 0x70, 0x71, 0x34, 0x7D, 0x96, 0x0E, 0x00, 0x92, 0xF4, 0x0C, 0x75
	};
	constexpr int inputSize = sizeof(input);
	std::string string(inputSize, '\0');
	for (int i = 0; i < inputSize; i++)
		string[i] = input[i];
	return string;
}

std::string DeflateDecompress::GetSalsaTestInput()
{
	uint8_t input[]
	{
		0x4B, 0x54 ,0x28 ,0x4E ,0xCC ,0x49 ,0x4C ,0xD1 ,0x51 ,0x48 ,0x04 ,0x31 ,0x8A ,0x13 ,0x21 ,0x8C ,0xA4 ,0xC4 ,0x12 ,0x10 ,0x23 ,0x29 ,0x31
		,0x0F ,0x08 ,0x01
	};
	constexpr int inputSize = sizeof(input);
	std::string string(inputSize, '\0');
	for (int i = 0; i < inputSize; i++)
		string[i] = input[i];
	return string;
}

std::string DeflateDecompress::GetLongDutchSentenceTest()
{
	uint8_t input[]
	{
		0x1D,0x8B,0xD1,0x09,0xC3,0x30,0x0C,0x44,0x57,0xB9,0x09,0x32,0x47,0x02,0x59,0x42,0xC5,0x47,0x2D,0xEC,0x4A,0xC1,0x72,0x13,0xC8,0xF4,0x55,0xFA,
		73,0x1C,0xBC,0xF7,0x56,0xE9,0xDD,0xA1,0x0D,0x2F,0x1A,0x76,0xFF,0x6A,0x60,0xE5,0xF8,0x88,0x2D,0xD8,0x1A,0x8A,0x18,0xDE,0x4C,0x12,0xD2,0x43,
		90,0x27,0x4D,0x72,0xC2,0x34,0xE7,0x14,0x69,0x0F,0x91,0xF2,0x97,0xAB,0xFB,0x81,0x8B,0x3D,0xAB,0x89,0xC2,0x9B,0x38,0x64,0x04,0x07,0xDC,0x9F,
		0xC8,0x22,0xE1,0x68,0x13,0x55,0xAB,0x2E,0x3F
	};
	constexpr int inputSize = sizeof(input);
	std::string string(inputSize, '\0');
	for (int i = 0; i < inputSize; i++)
		string[i] = input[i];
	return string;
}

std::string DeflateDecompress::GetLongDutchSentence2Test()
{
	//Hallo, ik ben Louis Herman. Ik probeer een decompressor te maken voor DEFLATE. Hopelijk lukt het ooit. Groetjes. Hihi
	uint8_t input[]
	{
		0x0D,0x8A,0xCB,0x0D,0x83,0x30,0x10,0x44,0x5B,0x99,0x02,0x22,0xF7,0x10,0x09,0x12,0x23,0x71,0x4C,0x03,0x26,0x8C,0xC4,0x62,0x9B,0xB5,0xD6,0x4B,0xEA,0x8F,0x8F,0xEF,0x13,0x53,0x29,
		0xFA,0x80,0x64,0x6C,0xBC,0xB0,0xEA,0x2D,0x1D,0x91,0x56,0xD3,0x15,0xB0,0x64,0x34,0xD3,0x8D,0x34,0x70,0xC4,0x9D,0x5F,0xAD,0xCD,0xD8,0xBB,0x1A,0x9C,0xA8,0x29,0x0F,0xFB,0xD3,0x41,
		0xD3,0xFC,0x5A,0x9F,0x9F,0x39,0x20,0x6A,0x63,0x91,0x33,0xA3,0xDC,0xD9,0x71,0xD0,0xA1,0x2A,0x1E,0xF0,0x36,0xA5,0x9F,0xEC,0xE3,0x90,0x43,0xFE
	};
	constexpr int inputSize = sizeof(input);
	std::string string(inputSize, '\0');
	for (int i = 0; i < inputSize; i++)
		string[i] = input[i];
	return string;
}

std::string DeflateDecompress::GetBlueTest()
{
	uint8_t input[]
	{
		0xED, 0x53, 0xBB, 0x6E, 0xC3, 0x30, 0x0C, 0xDC, 0xFD, 0x15, 0xDC, 0xB2, 0xE4, 0x27, 0xD2, 0x76, 0x09, 0xD0, 0xB1, 0x40, 0xD1, 0x51, 0x8E, 0x68, 0x4B, 0x8D, 0x2C, 0x06, 0x22, 0x6D, 0xC3, 0x7F, 0x5F, 0x4A, 0x8A, 0x13, 0x24, 0xFD, 0x80, 0x04, 0x45, 0x07, 0xC3,
		0x92, 0x78, 0xB8, 0xE3, 0xE3, 0xF8, 0x45, 0x5B, 0x08, 0x9E, 0x05, 0x23, 0x8C, 0x27, 0x70, 0x98, 0x70, 0xC3, 0x60, 0x80, 0x85, 0xD2, 0xD2, 0xEC, 0x5A, 0x1A, 0x45, 0x6F, 0xC1, 0x8B, 0x04, 0x84, 0x7E, 0x5C, 0x9A, 0x0F, 0x67, 0x44, 0xEF, 0x13, 0x32, 0xF8, 0xA8,
		0xA1, 0x36, 0x8C, 0x08, 0x33, 0xA5, 0x60, 0x9B, 0x5D, 0xB4, 0x60, 0x42, 0x00, 0x6B, 0x16, 0x30, 0xE7, 0x73, 0xF4, 0xBD, 0x93, 0x12, 0xC1, 0x09, 0xD3, 0x22, 0xCE, 0xC7, 0x5E, 0x55, 0x80, 0x31, 0x13, 0x30, 0x7C, 0x8F, 0x2C, 0x85, 0xA3, 0x79, 0xF7, 0x47, 0x04,
		0xE7, 0x07, 0xA5, 0x65, 0x6F, 0xB1, 0x30, 0xA8, 0x7A, 0x3E, 0x37, 0x2F, 0x59, 0xC4, 0x29, 0xDC, 0xD1, 0xC8, 0xD8, 0x7C, 0x7A, 0x71, 0xAB, 0xF4, 0x39, 0xB5, 0xD9, 0x47, 0x4B, 0x73, 0x4D, 0xA1, 0x06, 0x0E, 0x94, 0x26, 0x14, 0xC1, 0x7B, 0x71, 0x65, 0x29, 0xF1,
		0x8E, 0x52, 0x96, 0x2B, 0x61, 0xFD, 0x33, 0x86, 0xAE, 0x68, 0x16, 0x68, 0x4B, 0x56, 0x6B, 0x48, 0x34, 0x46, 0xDB, 0xBC, 0x1A, 0xD5, 0xCC, 0x39, 0x1B, 0x1F, 0x37, 0x02, 0x3D, 0x09, 0x44, 0x2A, 0x00, 0xA1, 0xB5, 0x75, 0x42, 0xCD, 0x7E, 0x33, 0xD4, 0x42, 0xDE,
		0x34, 0x01, 0x03, 0x16, 0x51, 0x1B, 0x51, 0x4E, 0xFE, 0x01, 0x4F, 0x4F, 0x96, 0x0D, 0x38, 0x33, 0x69, 0x03, 0x6B, 0xE7, 0x7F, 0x0F, 0xF1, 0x3C, 0xBD, 0x32, 0x66, 0x9D, 0x8F, 0x68, 0xB3, 0x0F, 0x14, 0x68, 0x4C, 0x40, 0x5D, 0xB1, 0x91, 0x64, 0xD7, 0xED, 0x61,
		0x46, 0x93, 0x2A, 0xCA, 0x24, 0x2C, 0x30, 0x96, 0x84, 0x28, 0x7C, 0xF1, 0x5E, 0x7E, 0xCB, 0x4F, 0x5C, 0x11, 0x44, 0x57, 0xED, 0xDE, 0xA7, 0xD0, 0x25, 0x8F, 0x19, 0xA9, 0x1F, 0xBB, 0xA2, 0xC5, 0x54, 0xFB, 0x74, 0xC3, 0x7A, 0x42, 0x3A, 0xA9, 0xA9, 0xF2, 0x36,
		0x54, 0xC3, 0xCF, 0x26, 0x1C, 0x57, 0x3F, 0x14, 0x64, 0xC8, 0x6E, 0x1D, 0x96, 0x8B, 0xCB, 0xC0, 0x4B, 0x5D, 0x88, 0x7B, 0xD7, 0xAE, 0x94, 0xBA, 0x22, 0x96, 0xB5, 0x04, 0x36, 0xBA, 0x56, 0x0A, 0x9A, 0x6B, 0x45, 0xD9, 0x93, 0xC7, 0x5B, 0x64, 0x87, 0x18, 0xD4,
		0xA8, 0x7C, 0x5D, 0xB5, 0x75, 0x25, 0x06, 0x7C, 0xB6, 0xB9, 0x3E, 0x57, 0x36, 0x7F, 0xDE, 0x65, 0xDB, 0x7F, 0x9B, 0xB5, 0x8F, 0xB6, 0xD9, 0x0F
	};
	constexpr int inputSize = sizeof(input);
	std::string string(inputSize, '\0');
	for (int i = 0; i < inputSize; i++)
		string[i] = input[i];
	return string;
}

bool DeflateDecompress::ValidateInternetExampleOutput(const std::string& m_Output)
{
	constexpr uint8_t desiredOutput[]
	{
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0xc0,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x40,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x40,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x40,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0xc0,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x40,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0x40,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0x40,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x40,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x40
	};
	constexpr int inputSize = sizeof(desiredOutput);
	if (inputSize != m_Output.size())
		return false;
	for (int i = 0; i < inputSize; i++)
		if (m_Output[i] != desiredOutput[i])
			return false;
	return true;
}

bool DeflateDecompress::ValidateMyTest1Output(const std::string& m_Output)
{
	constexpr uint8_t desired[]
	{
		0x76, 0x111, 0x117 ,0x105, 0x115 ,0x32, 0x72 ,0x101 ,0x114, 0x109, 0x97 ,0x110
	};
	constexpr int inputSize = sizeof(desired);
	if (inputSize != m_Output.size())
		return false;
	for (int i = 0; i < inputSize; i++)
		if (m_Output[i] != desired[i])
			return false;
	return true;
}