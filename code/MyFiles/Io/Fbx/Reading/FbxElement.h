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

					//---| Functions |---
					void SetName(std::string name);
					void AddProperty(FbxProperty* pProperty);

					FbxElement& CreateChild();

					const List<FbxElement>& GetChildren() const { return m_Children; }
					const List<FbxProperty*>& GetProperty() const { return m_Properties; }
					const std::string& GetName() const { return m_Name; }

				private:
					std::string m_Name{};
					List<FbxElement> m_Children{0};
					List<FbxProperty*> m_Properties;
				};
			}
		}
	}
}
