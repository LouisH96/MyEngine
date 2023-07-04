#pragma once
#include <istream>

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				template<typename>	class FbxPropArray;
				template<typename> class FbxPropPrimitive;
				class FbxPropRaw;
				class FbxPropString;

				class FbxProperty
				{
				public:
					FbxProperty() = default;
					virtual ~FbxProperty() = default;

					virtual void Print(bool compact = false, int nrTabs = 0) const = 0;
					static FbxProperty* Read(std::istream& stream);

					const FbxPropRaw& AsRaw() const;
					const FbxPropString& AsStringProp() const;
					const std::string& AsString() const;
					template<typename T> const FbxPropPrimitive<T>& AsPrimitive() const;
					template<typename T> const FbxPropArray<T>& AsArray() const;
					FbxPropRaw& AsRaw();
					FbxPropString& AsStringProp();
					std::string& AsString();
					template<typename T> FbxPropPrimitive<T>& AsPrimitive();
					template<typename T> FbxPropArray<T>& AsArray();

				protected:
					void BeginPrint(int nrTabs) const;
				};

				template <typename T>
				const FbxPropPrimitive<T>& FbxProperty::AsPrimitive() const
				{
					return *reinterpret_cast<const FbxPropPrimitive<T>*>(this);
				}

				template <typename T>
				const FbxPropArray<T>& FbxProperty::AsArray() const
				{
					return *reinterpret_cast<const FbxPropArray<T>*>(this);
				}

				template <typename T>
				FbxPropPrimitive<T>& FbxProperty::AsPrimitive()
				{
					return *reinterpret_cast<FbxPropPrimitive<T>*>(this);
				}

				template <typename T>
				FbxPropArray<T>& FbxProperty::AsArray()
				{
					return *reinterpret_cast<FbxPropArray<T>*>(this);
				}
			}
		}
	}
}
