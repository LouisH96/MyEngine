#pragma once
#include <istream>
#include <vector>
#include <Io/Fbx/Reading/FbxProperty.h>

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				class FbxObject
				{
				public:
					//---| Construction |---
					FbxObject(std::istream& stream, uint8_t version, bool isRoot = false);
					~FbxObject();

					FbxObject(const FbxObject& other) = delete;
					FbxObject& operator= (const FbxObject& other) = delete;
					FbxObject(FbxObject&& other) = default;
					FbxObject& operator= (FbxObject&& other) = default;

					//---| Functions |---
					std::streampos GetEnd() const { return m_End; }

					void Print(bool compact = true, int nrTabs = 0) const;
					const FbxObject* GetChild(const std::string& name) const;
					const FbxObject& GetChild(int idx) const;
					const std::vector<FbxObject*>& GetChildren() const { return m_Children; }
					std::vector<FbxObject*> GetChildren(const std::string& name) const;
					const FbxProperty* GetProperty(int idx) const;
					const std::vector<FbxProperty*>& GetProperties() const { return m_Properties; }
					const FbxProperty& GetLastProperty() const;
					FbxObject* GetChild(const std::string& name);
					FbxObject& GetChild(int idx);
					FbxProperty* GetProperty(int idx);
					const std::string& GetName() const { return m_Name; }
					int GetNrProperties() const { return m_Properties.size(); }
					int GetNrChildren() const { return m_Children.size(); }

				private:
					std::string m_Name;
					std::streampos m_End;
					std::vector<FbxObject*> m_Children{};
					std::vector<FbxProperty*> m_Properties{};

					uint64_t m_NrProps{};
					uint64_t m_PropLength{};

					void ReadNode(std::istream& stream, uint8_t version);
				};
			}
		}
	}
}
