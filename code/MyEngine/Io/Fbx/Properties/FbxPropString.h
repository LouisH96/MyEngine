#pragma once
#include <Io\Fbx\FbxProperty.h>

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			class FbxPropString
				: public FbxProperty
			{
			public:
				FbxPropString(std::istream& stream);

				void Print(bool compact, int nrTabs) const override;
				const std::string& GetValue() const { return m_String; }
				static std::string Read(std::istream& stream);
			private:
				const std::string m_String;
			};
		}
	}
}
