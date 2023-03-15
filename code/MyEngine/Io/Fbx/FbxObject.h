#pragma once
#include <vector>
#include <Io\Fbx\FbxProperty.h>

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			class FbxObject
			{
			public:
				FbxObject(std::istream& stream);
				~FbxObject();

				std::streampos GetEnd() const { return m_End; }

				void Print(int nrTabs = 0);

			private:
				std::string m_Name;
				std::streampos m_End;
				std::vector<FbxObject*> m_Children{};
				std::vector<FbxProperty*> m_Properties{};

				int m_NrProps{};
				int m_PropLength{};

				void ReadNode(std::istream& stream);
			};
		}
	}
}
