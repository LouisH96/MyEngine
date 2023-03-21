#pragma once

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
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
				template<typename T> const FbxPropPrimitive<T>& AsPrimitive() const;
				template<typename T> const FbxPropArray<T>& AsArray() const;
				const std::string& AsString() const;

			protected:
				void BeginPrint(int nrTabs) const;
			};

			template <typename T>
			const Io::Fbx::FbxPropPrimitive<T>& Io::Fbx::FbxProperty::AsPrimitive() const
			{
				return *reinterpret_cast<const FbxPropPrimitive<T>*>(this);
			}

			template <typename T>
			const Io::Fbx::FbxPropArray<T>& Io::Fbx::FbxProperty::AsArray() const
			{
				return *reinterpret_cast<const FbxPropArray<T>*>(this);
			}
		}
	}
}
