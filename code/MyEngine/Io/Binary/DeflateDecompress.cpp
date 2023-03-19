#include "pch.h"
#include "DeflateDecompress.h"

#include <vector>

#include "BitStream.h"

using namespace Io::Binary;



DeflateDecompress::DeflateDecompress(std::istream& stream)
	: m_Input(stream)
	, m_BitStream(stream)
	, m_Output()
{
	const uint8_t isEnd = m_BitStream.ReadBits(1);
	const uint8_t bType = m_BitStream.ReadBits(1) + (m_BitStream.ReadBits(1) << 1);

	if (bType == 0)
	{
		Logger::PrintError("Block has no compression, which is not supported yet");
		return;
	}
	if (bType == 2)
	{
		Logger::PrintError("Block has dynamic Huffman codes, which is not supported yet");
		return;
	}
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

	for (int i = 0; i < m_Output.size(); i++)
		std::cout << m_Output[i];
	//std::cout << "0x" << std::hex << static_cast<int>(m_Output[i]) << std::endl;
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

uint16_t DeflateDecompress::ReadLength(uint16_t code)
{
	if (code <= 264)
		return code - 257 + 3;
	if (code == 258)
		return 258;
	const uint8_t base = code - 261;
	const uint8_t nrExtraBits = base / 4;
	const uint8_t modulo = base % 4;
	const uint8_t extraBitsValue = m_BitStream.ReadBits(nrExtraBits);
	uint16_t length = (1 << nrExtraBits) - 1 << 2;
	length += (1 << nrExtraBits) * modulo;
	length += 7;
	length += extraBitsValue;
	return length;
}

uint16_t DeflateDecompress::ReadDistance(uint8_t code)
{
	if (code <= 3)
		return code + 1;
	const uint8_t base = code - 2;
	const uint8_t nrExtraBits = base / 2;
	const uint8_t modulo = base % 2;
	const uint16_t extraBitsValue = m_BitStream.ReadBits<uint16_t>(nrExtraBits);
	uint16_t distance = (1 << nrExtraBits - 1) - 1 << 2;
	distance += modulo << nrExtraBits;
	distance += 5;
	distance += extraBitsValue;
	return distance;
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