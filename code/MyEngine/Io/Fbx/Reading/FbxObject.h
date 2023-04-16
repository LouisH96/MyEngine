#pragma once
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
					FbxObject(std::istream& stream, uint8_t version, bool isRoot = false);
					~FbxObject();

					std::streampos GetEnd() const { return m_End; }

					void Print(bool compact = true, int nrTabs = 0) const;
					const FbxObject* GetChild(const std::string& name) const;
					const std::vector<FbxObject*>& GetChildren() const { return m_Children; }
					std::vector<FbxObject*> GetChildren(const std::string& name) const;
					const FbxProperty* GetProperty(int idx) const;
					const std::vector<FbxProperty*>& GetProperties() const { return m_Properties; }
					FbxObject* GetChild(const std::string& name);
					FbxProperty* GetProperty(int idx);
					const std::string& GetName() const { return m_Name; }

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
