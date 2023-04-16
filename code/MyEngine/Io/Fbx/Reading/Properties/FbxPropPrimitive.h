#pragma once
#include <Io/Fbx/Reading/FbxProperty.h>

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				template<typename T>
				class FbxPropPrimitive
					: public FbxProperty
				{
				public:
					FbxPropPrimitive(std::istream& s);

					T GetValue() const { return m_Value; }
					void Print(bool compact, int nrTabs) const override;
					static T Read(std::istream& stream);
					static std::string TypeToString();
				private:
					const T m_Value;
				};

				template<typename T>
				inline FbxPropPrimitive<T>::FbxPropPrimitive(std::istream& s)
					: m_Value(Read(s))
				{
				}
				template<typename T>
				inline void FbxPropPrimitive<T>::Print(bool compact, int nrTabs) const
				{
					BeginPrint(nrTabs);
					std::cout << "[" << TypeToString() << "] " << std::to_string(m_Value) << std::endl;
				}
				template<typename T>
				inline T FbxPropPrimitive<T>::Read(std::istream& stream)
				{
					unsigned char c[sizeof(T)];
					for (int i = 0; i < sizeof(T); i++)
						c[i] = static_cast<unsigned char>(stream.get());
					return T{ *reinterpret_cast<T*>(&c[0]) };
				}
				template<>
				inline std::string FbxPropPrimitive<bool>::TypeToString()
				{
					return "bool";
				}
				template<>
				inline std::string FbxPropPrimitive<uint16_t>::TypeToString()
				{
					return "int16";
				}
				template<>
				inline std::string FbxPropPrimitive<uint32_t>::TypeToString()
				{
					return "int";
				}
				template<>
				inline std::string FbxPropPrimitive<float>::TypeToString()
				{
					return "float";
				}
				template<>
				inline std::string FbxPropPrimitive<double>::TypeToString()
				{
					return "double";
				}
				template<>
				inline std::string FbxPropPrimitive<uint64_t>::TypeToString()
				{
					return "int64";
				}
				template<typename T>
				inline std::string FbxPropPrimitive<T>::TypeToString()
				{
					return "unknownType";
				}
			}
		}
	}
}

