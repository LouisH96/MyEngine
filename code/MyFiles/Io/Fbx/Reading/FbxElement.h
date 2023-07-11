#pragma once
#include <string>

#include "DataStructures/List.h"

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
					void Print(bool compact = true, unsigned nrTabs = 0) const;

				private:
					std::string m_Name{};
					List<FbxElement> m_Children{0};
					List<FbxProperty*> m_Properties;
				};
			}
		}
	}
}
