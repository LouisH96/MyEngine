#pragma once
#include <fstream>
#include <sstream>
#include <Io/Binary/Bini.h>
#include <Io/Fbx/FbxProperty.h>
#include <Io/Fbx/Properties/FbxPropPrimitive.h>
#include <Io/Zlib/ZlibDecompress.h>
#include <Io/Binary/Deflate.h>

using namespace Io::Binary;

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			template<typename T>
			class FbxPropArray
				: public FbxProperty
			{
			public:
				FbxPropArray(std::istream& stream);

				void Print(int nrTabs) const override;
			private:
				Array<T> m_Values;
				bool m_IsCompressed;

				void HandleUncompressed(std::istream& stream, unsigned nrElements);
				void HandleCompressed(std::istream& stream, unsigned byteLength);
			};
			template<typename T>
			inline FbxPropArray<T>::FbxPropArray(std::istream& stream)
			{
				const unsigned nrElements = Bini::Uint32(stream);
				const unsigned encoding = Bini::Uint32(stream);
				const unsigned compressedLength = Bini::Uint32(stream);
				m_IsCompressed = encoding == 1;
				if (m_IsCompressed)
					HandleCompressed(stream, compressedLength);
				else
					HandleUncompressed(stream, nrElements);
			}
			template<typename T>
			inline void FbxPropArray<T>::Print(int nrTabs) const
			{
				BeginPrint(nrTabs);
				std::cout << "[" << FbxPropPrimitive<T>::TypeToString() << "] ";
				if (m_IsCompressed)
					std::cout << "compressed array\n";
				else
					std::cout << "array(" << m_Values.GetSize() << ")\n";
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
				std::string myInput = Binary::Deflate::GetSalsaTestInput();
				std::istringstream myInputStream(myInput);

				//Zlib::ZlibDecompress::Unzip{ myInputStream };
				Binary::Deflate::Decompress(myInputStream);

				//Zlib::ZlibDecompress decompress{ stream };
				stream.seekg(end);
			}
		}
	}
}
