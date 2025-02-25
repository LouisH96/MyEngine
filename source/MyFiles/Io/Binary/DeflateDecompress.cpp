#include "DeflateDecompress.h"

#include <vector>
#include <Io/Binary/Huffman.h>

#include "BitStream.h"

//#define DEFLATE_DECOMPRESS_DEBUG

using namespace MyEngine::Io::Binary;

std::vector<uint8_t> DeflateDecompress::Decompress(std::istream& stream)
{
	DeflateDecompress c{ stream };
	return c.m_Output;
}

DeflateDecompress::DeflateDecompress(std::istream& stream)
	: m_Input(stream)
	, m_BitStream(stream)
	, m_Output()
{
	uint8_t isEnd = 0;
	while (isEnd == 0)
	{
		isEnd = m_BitStream.ReadBits(1);
		const uint8_t bType = m_BitStream.ReadBits(1) + (m_BitStream.ReadBits(1) << 1);

		switch (bType)
		{
		case 1: HandleFixedBlock(); break;
		case 2: HandleDynamicBlock(); break;
		case 0: HandleNoCompression(); return;
		case 3: Logger::Error("Block-type was 3 which shouldn't be used"); return;
		default: Logger::Error("Unknown block-type (BTYPE) of " + std::to_string(bType)); return;
		}
	}
}


void DeflateDecompress::HandleNoCompression()
{
	m_BitStream.ToNextByte();
	const uint16_t length{ m_BitStream.ReadBitsFlipped<uint16_t>(16) };
	const int16_t lengthNegate{ m_BitStream.ReadBitsFlipped<int16_t>(16) };
	for (int i = 0; i < length; i++)
		m_Output.push_back(m_BitStream.ReadBitsFlipped<uint8_t>(8));
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
			HandleLengthDistancePair(next8 - 0b1100'0000 + 280);
			continue;
		}
		//literal
		const uint16_t next9 = next8 << 1 | m_BitStream.ReadBits(1);
		m_Output.push_back(next9 - 0b1'1001'0000 + 144);
	}
}

void DeflateDecompress::HandleLengthDistancePair(uint16_t lengthCode)
{
#ifdef DEFLATE_DECOMPRESS_DEBUG
	if (lengthCode < 257 || lengthCode >285)
	{
		Logger::Error("length-code(" + std::to_string(lengthCode) + ") should be [257-285]");
		throw 0;
	}
#endif

	const uint16_t length = ReadLength(lengthCode, true);
	const uint8_t distanceCode = m_BitStream.ReadBits(5);
	const uint16_t distance = ReadDistance(distanceCode, true);

	std::vector<uint8_t> newChars{};
	const unsigned begin = static_cast<unsigned>(m_Output.size()) - distance;
	const unsigned end = begin + length;
	for (unsigned i = begin; i < end; i++)
	{
		const unsigned outputIdx = begin + ((i - begin) % (m_Output.size() - begin));
		newChars.push_back(m_Output[outputIdx]);
	}
	for (unsigned i = 0; i < newChars.size(); i++)
		m_Output.push_back(newChars[i]);
}

uint16_t DeflateDecompress::ReadLength(uint16_t code, bool flipped)
{
	if (code <= 264)
		return code - 257 + 3;
	if (code == 285)
		return 258;
	const uint8_t base = code - 261u;
	const uint8_t nrExtraBits = base / 4u;
	const uint8_t modulo = base % 4u;
	const uint8_t extraBitsValue = flipped ? m_BitStream.ReadBitsFlipped(nrExtraBits) : m_BitStream.ReadBits(nrExtraBits);
	unsigned length{ ((1u << nrExtraBits) - 1u) << 2u };
	length += (1u << nrExtraBits) * modulo;
	length += 7u;
	length += extraBitsValue;
	return static_cast<uint16_t>(length);
}

uint16_t DeflateDecompress::ReadDistance(uint8_t code, bool flipped)
{
	if (code <= 3)
		return code + 1;
	const uint8_t base = code - 2;
	const uint8_t nrExtraBits = base / 2;
	const uint8_t modulo = base % 2;
	const uint16_t extraBitsValue = flipped ? m_BitStream.ReadBitsFlipped<uint16_t>(nrExtraBits) : m_BitStream.ReadBits<uint16_t>(nrExtraBits);
	unsigned distance{ ((1u << (nrExtraBits - 1u)) - 1u) << 2u };
	distance += modulo << nrExtraBits;
	distance += 5;
	distance += extraBitsValue;
	return static_cast<uint16_t>(distance);
}

void DeflateDecompress::HandleDynamicBlock()
{
	const uint8_t hLit{ m_BitStream.ReadBitsFlipped(5) };
	const uint8_t hDist{ m_BitStream.ReadBitsFlipped(5) };
	const uint8_t hClen{ m_BitStream.ReadBitsFlipped(4) };

	Array<uint8_t> clSymbolCodeLengths{ 19 };
	ReadClCodeLengths(hClen, clSymbolCodeLengths);
	Array<uint16_t> clSymbolCodes = Huffman<uint16_t>::GetCodesFromCodeLengths(clSymbolCodeLengths);

	Array<uint8_t> llCodeLengths{ 286, 0 };
	GetCodeLengths(llCodeLengths, clSymbolCodes, clSymbolCodeLengths, hLit + 257);
	Array<uint8_t> dCodeLengths{ 32, 0 };
	GetCodeLengths(dCodeLengths, clSymbolCodes, clSymbolCodeLengths, hDist + 1);

	const Array<uint16_t> llCodes = Huffman<uint16_t>::GetCodesFromCodeLengths(llCodeLengths);
	const Array<uint16_t> dCodes = Huffman<uint16_t>::GetCodesFromCodeLengths(dCodeLengths);

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

void DeflateDecompress::GetCodeLengths(Array<uint8_t>& output, const Array<uint16_t>& clSymbols, const Array<uint8_t>& clSymbolsLengths,
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
		Logger::Error("Unknown codeLengthSymbolCode found");
		return;
	}
}

uint8_t DeflateDecompress::GetNextClSymbol(const Array<uint16_t>& clSymbolCodes,
	const Array<uint8_t>& clSymbolCodeLengths)
{
	uint8_t previousLength = 0;
	uint8_t readValue = 0;

	while (true)
	{
		//find shortest
		uint8_t shortest{ 255u };
		for (unsigned i = 0; i < clSymbolCodeLengths.GetSize(); i++)
			if (clSymbolCodeLengths[i] > previousLength && clSymbolCodeLengths[i] < shortest)
				shortest = clSymbolCodeLengths[i];
		if (shortest == 255u)
		{
			Logger::Error("No shorter code found");
			return 0;
		}

		//try find valid clSymbolCode)
		readValue <<= shortest - previousLength;
		readValue |= m_BitStream.ReadBits(shortest - previousLength);

		for (unsigned i = 0; i < clSymbolCodeLengths.GetSize(); i++)
		{
			if (clSymbolCodeLengths[i] == shortest &&
				clSymbolCodes[i] == readValue)
				return i;
		}
		previousLength = shortest;
	}
}

void DeflateDecompress::CopyToOutput(Array<uint8_t>& output, uint32_t startIdx, uint8_t value, uint8_t amount)
{
	for (unsigned i = startIdx; i < startIdx + amount; i++)
		output[i] = value;
}

void DeflateDecompress::CopyToOutput(Array<uint16_t>& output, uint32_t startIdx, uint16_t value, uint8_t amount)
{
	for (unsigned i = startIdx; i < startIdx + amount; i++)
		output[i] = value;
}

void DeflateDecompress::ReadDynamicBlockData(const Array<uint16_t>& llCodes, const Array<uint8_t>& llCodeLengths,
	const Array<uint16_t>& dCodes, const Array<uint8_t>& dCodeLengths)
{
	while (true)
	{
		const uint16_t next = GetNextCl(llCodes, llCodeLengths);
		if (next <= 255)
		{
			m_Output.push_back(static_cast<uint8_t>(next));
			continue;
		}
		if (next == 256)
			return;
		if (next <= 285)
		{
			const uint16_t length = ReadLength(next, true);
			const uint16_t distanceCode = GetNextCl(dCodes, dCodeLengths);
			const uint16_t distance = ReadDistance(static_cast<uint8_t>(distanceCode), true);

			std::vector<uint8_t> newChars{};
			const unsigned begin = static_cast<unsigned>(m_Output.size()) - distance;
			const unsigned end = begin + length;
			for (unsigned i = begin; i < end; i++)
			{
				const unsigned outputIdx = begin + ((i - begin) % (m_Output.size() - begin));
				newChars.push_back(m_Output[outputIdx]);
			}
			for (unsigned i = 0; i < newChars.size(); i++)
				m_Output.push_back(newChars[i]);
			continue;
		}
		Logger::Error("Unknown code");
		return;
	}
}

uint16_t DeflateDecompress::GetNextCl(const Array<uint16_t>& codes, const Array<uint8_t>& codeLengths)
{
	uint8_t previousLength = 0;
	uint16_t readValue = 0;

	while (true)
	{
		//find shortest
		uint8_t shortest{ 255u };
		for (unsigned i = 0; i < codeLengths.GetSize(); i++)
			if (codeLengths[i] > previousLength && codeLengths[i] < shortest)
				shortest = codeLengths[i];
		if (shortest == 255)
		{
			Logger::Error("No shorter code found");
			return 0;
		}

		//try find valid clSymbolCode)
		readValue <<= shortest - previousLength;
		readValue |= m_BitStream.ReadBits<uint16_t>(shortest - previousLength);

		for (unsigned i = 0; i < codeLengths.GetSize(); i++)
		{
			if (codeLengths[i] == shortest &&
				codes[i] == readValue)
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

		const uint16_t groupBase{ static_cast<uint16_t>(((1u << divide) - 1u) << 2u) };
		const uint16_t addition{ static_cast<uint16_t>((1u << divide) * modulo) };
		const uint16_t beginRange{ static_cast<uint16_t>(groupBase + addition + 7u) };
		const uint16_t endRange{ static_cast<uint16_t>(beginRange + ((1u << divide) - 1u)) };

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

		const uint16_t divideBase{ static_cast<uint16_t>(((1u << (nrBits - 1u)) - 1u) << 2u) };
		const uint16_t moduloAdd{ static_cast<uint16_t>(modulo << nrBits) };
		const uint16_t beginRange{ static_cast<uint16_t>(divideBase + moduloAdd + 5u) };
		const uint16_t endRange{ static_cast<uint16_t>(beginRange + ((1 << nrBits) - 1u)) };

		std::cout << i << "\t|" << std::to_string(nrBits) << "\t|" << beginRange << "-" << endRange << std::endl;
	}
}
