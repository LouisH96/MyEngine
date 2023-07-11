#pragma once
#include <string>

#include "FbxProperty.h"
#include "DataStructures/Array.h"
#include "DataStructures/List.h"
#include "Logger/Logger.h"
#include "Properties/FbxPropArray.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				class FbxProperty;

				class FbxElement
				{
				public:
					//---| Construction |---
					explicit FbxElement(std::string name);
					FbxElement() = default;
					~FbxElement();

					FbxElement(const FbxElement& other) = delete;
					FbxElement& operator=(const FbxElement& other) = delete;
					FbxElement(FbxElement&& other) noexcept;
					FbxElement& operator=(FbxElement&& other) noexcept;

					//---| Creation |---
					void SetName(std::string name);
					void AddProperty(FbxProperty* pProperty);
					void EnsurePropertyCapacity(unsigned minCapacity);
					FbxElement& CreateChild();

					//---| Get Children |---
					const List<FbxElement>& GetChildren() const { return m_Children; }
					List<const FbxElement*> GetChildren(const std::string& name) const;
					const FbxElement& GetChild(unsigned idx) const;
					const FbxElement* GetChild(const std::string& name) const;
					unsigned GetNrChildren() const { return m_Children.GetSizeU(); }

					List<FbxElement>& GetChildren() { return m_Children; }
					List<FbxElement*> GetChildren(const std::string& name);
					FbxElement& GetChild(unsigned idx);
					FbxElement* GetChild(const std::string& name);

					//---| Get Properties |---
					const List<FbxProperty*>& GetProperties() const { return m_Properties; }
					const FbxProperty& GetProperty(unsigned idx) const;
					const FbxProperty& GetLastProperty() const;
					unsigned GetNrProperties() const { return m_Properties.GetSizeU(); }

					List<FbxProperty*>& GetProperties() { return m_Properties; }
					FbxProperty& GetProperty(unsigned idx);
					FbxProperty& GetLastProperty();

					//---| Other |---
					const std::string& GetName() const { return m_Name; }
					std::string& GetName() { return m_Name; }
					void Print(bool compact = true, unsigned nrTabs = 0) const;

					template<typename T>
					bool EnsureArrayPropertyType(unsigned propertyIdx);

				private:
					std::string m_Name{};
					List<FbxElement> m_Children{0};
					List<FbxProperty*> m_Properties;

					template<typename Old, typename New>
					void ReplaceArrayProperty(unsigned propIdx);
				};

				template <typename T>
				bool FbxElement::EnsureArrayPropertyType(unsigned propertyIdx)
				{
					FbxProperty* pProperty{ m_Properties[propertyIdx] };

					if (dynamic_cast<FbxPropArray<T>*>(pProperty))
						return false;

					if (dynamic_cast<FbxPropArray<double>*>(pProperty))
					{
						ReplaceArrayProperty<double, T>(propertyIdx);
						return true;
					}
					Logger::PrintError("[FbxElement::EnsureArrayPropertyType] unsupported type");
					return true;
				}

				template <typename Old, typename New>
				void FbxElement::ReplaceArrayProperty(unsigned propIdx)
				{
					FbxProperty* pOld{ m_Properties[propIdx] };

					Array<Old>& oldArray{pOld->AsArray<Old>().GetValues()};
					Array<New> newArray{oldArray.GetSizeU()};

					for (unsigned i = 0; i < newArray.GetSizeU(); i++)
						newArray[i] = static_cast<New>(oldArray[i]);
					
					FbxPropArray<New>* pNew{new FbxPropArray<New>()};
					pNew->SetArray(std::move(newArray));

					m_Properties[propIdx] = pNew;
					delete pOld;
				}
			}
		}
	}
}
