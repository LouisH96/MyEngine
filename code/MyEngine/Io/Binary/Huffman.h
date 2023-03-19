#pragma once
#include <DataStructures/Algorithms.h>

namespace MyEngine
{
	namespace Io
	{
		namespace Binary
		{
			template<typename Code>
			class Huffman
			{
			public:
				Huffman() = delete;

				static Array<Code> GetCodesFromCodeLengths(const Array<uint8_t>& codeLengths);
			};

			template <typename Code>
			Array<Code> Huffman<Code>::GetCodesFromCodeLengths(const Array<uint8_t>& codeLengths)
			{
				//step 1
				const uint8_t highestNrOfCl{ Algorithms::GetBiggest(codeLengths) };
				Array<uint8_t> clCount{ highestNrOfCl + 1, 0 };
				for (int i = 0; i < codeLengths.GetSize(); i++)
					if (codeLengths[i] > 0)
						clCount[codeLengths[i]]++;

				//step 2
				Array<uint8_t> codeOffsets{ highestNrOfCl+1, 0 };
				uint8_t code = 0;
				uint8_t prevCount = 0;
				for (int i = 1; i < codeOffsets.GetSize(); i++)
					//if (clCount[i] > 0)
					{
						code = (code + prevCount) << 1;
						codeOffsets[i] = code;
						prevCount = clCount[i];
					}

				//step 3
				Array<uint8_t> codes{ codeLengths.GetSize(), 0 };
				for (int i = 0; i < codes.GetSize(); i++)
					if (codeLengths[i] > 0)
					{
						codes[i] = codeOffsets[codeLengths[i]];
						codeOffsets[codeLengths[i]]++;
					}

				return codes;
			}
		}
	}
}
