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
				Array<Code> clCount{ highestNrOfCl + 1, 0 };
				for (unsigned i = 0; i < codeLengths.GetSize(); i++)
					if (codeLengths[i] > 0)
						++clCount[codeLengths[i]];

				//step 2
				Array<Code> codeOffsets{ highestNrOfCl+1, 0 };
				Code code = 0;
				uint8_t prevCount = 0;
				for (unsigned i = 1; i < codeOffsets.GetSize(); i++)
					//if (clCount[i] > 0)
					{
						code = static_cast<Code>((code + static_cast<unsigned>(prevCount)) << 1u);
						codeOffsets[i] = code;
						prevCount = static_cast<uint8_t>(clCount[i]);
					}

				//step 3
				Array<Code> codes{ codeLengths.GetSize(), 0 };
				for (unsigned i = 0; i < codes.GetSize(); i++)
					if (codeLengths[i] > 0)
					{
						codes[i] = codeOffsets[codeLengths[i]];
						++codeOffsets[codeLengths[i]];
					}

				return codes;
			}
		}
	}
}
