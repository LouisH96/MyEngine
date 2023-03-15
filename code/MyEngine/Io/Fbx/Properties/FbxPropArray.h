#pragma once
#include <Io\Fbx\FbxProperty.h>
#include <Io\Fbx\Properties\FbxPropPrimitive.h>
#include <Io\Binary\Bini.h>

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
			};
			template<typename T>
			inline FbxPropArray<T>::FbxPropArray(std::istream& stream)
			{
				const unsigned nrElements = Bini::Uint32(stream);
				const unsigned encoding = Bini::Uint32(stream);
				const unsigned compressedLength = Bini::Uint32(stream);

				if (encoding == 1)
				{
					m_IsCompressed = true;
					stream.ignore(compressedLength);
					return;
				}
				m_IsCompressed = false;
				m_Values = { nrElements };
				for (int i = 0; i < nrElements; i++)
					m_Values[i] = FbxPropPrimitive<T>::Read(stream);
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
		}
	}
}
