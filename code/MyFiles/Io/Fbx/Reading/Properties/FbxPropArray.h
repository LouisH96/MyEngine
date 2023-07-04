#pragma once
#include <sstream>
#include <Io/Binary/LittleEndianReader.h>
#include <Io/Fbx/Reading/FbxProperty.h>
#include <Io/Fbx/Reading/Properties/FbxPropPrimitive.h>
#include <Io/Zlib/ZlibDecompress.h>

#include "DataStructures/Array.h"
#include "Logger/Logger.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				template<typename T>
				class FbxPropArray
					: public FbxProperty
				{
				public:
					FbxPropArray(std::istream& stream);

					const Array<T>& GetValues() const { return m_Values; }
					Array<T>& GetValues() { return m_Values; }

					void Print(bool compact, int nrTabs) const override;
				private:
					Array<T> m_Values;
					bool m_IsCompressed;

					void HandleUncompressed(std::istream& stream, unsigned nrElements);
					void HandleCompressed(std::istream& stream, unsigned byteLength);
				};
				template<typename T>
				inline FbxPropArray<T>::FbxPropArray(std::istream& stream)
				{
					const unsigned nrElements = Binary::LittleEndianReader::Uint32(stream);
					const unsigned encoding = Binary::LittleEndianReader::Uint32(stream);
					const unsigned compressedLength = Binary::LittleEndianReader::Uint32(stream);
					m_IsCompressed = encoding == 1;
					if (m_IsCompressed)
						HandleCompressed(stream, compressedLength);
					else
						HandleUncompressed(stream, nrElements);
				}
				template<typename T>
				inline void FbxPropArray<T>::Print(bool compact, int nrTabs) const
				{
					BeginPrint(nrTabs);
					std::cout << "[" << FbxPropPrimitive<T>::TypeToString() << "] ";
					if (compact)
						std::cout << "Array of " << m_Values.GetSize() << " elements\n";
					else
						Logger::Print("", m_Values);
				}

				template <typename T>
				void FbxPropArray<T>::HandleUncompressed(std::istream& stream, unsigned nrElements)
				{
					m_Values = { nrElements };
					for (int i = 0; i < nrElements; i++)
						m_Values[i] = FbxPropPrimitive<T>::Read(stream);
				}

				template <typename T>
				void FbxPropArray<T>::HandleCompressed(std::istream& stream, unsigned byteLength)
				{
					const unsigned end = static_cast<unsigned>(stream.tellg()) + byteLength;

					//std::string myInput = Zlib::ZlibDecompress::GetInternetExampleInput();
					//std::string myInput = Binary::DeflateDecompress::GetSalsaTestInput();
					//std::string myInput = Binary::DeflateDecompress::GetLongDutchSentence2Test();
					//std::string myInput = Binary::DeflateDecompress::GetBlueTest();
					//std::istringstream myInputStream(myInput);

					//Zlib::ZlibDecompress::Unzip(myInputStream);
					//Binary::DeflateDecompress{ myInputStream };

					const std::vector<uint8_t> data = Zlib::ZlibDecompress::Unzip(stream);

					m_Values = Array<T>(static_cast<int>(data.size() / sizeof(T)));
					for (int iDataByte = 0; iDataByte < data.size(); iDataByte += sizeof(T))
					{
						const int elementIdx = iDataByte / sizeof(T);
						T value{};
						uint8_t* pValue = reinterpret_cast<uint8_t*>(&value);
						for (int iElementByte = 0; iElementByte < sizeof(T); iElementByte++)
						{
							*pValue = data[iDataByte + iElementByte];
							pValue += 1;
						}
						m_Values[elementIdx] = value;
					}

					stream.seekg(end);
				}
			}
		}
	}
}
