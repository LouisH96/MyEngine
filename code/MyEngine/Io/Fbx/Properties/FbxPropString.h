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

				void Print(int nrTabs) override;
				static std::string Read(std::istream& stream);
			private:
				const std::string m_String;
			};
		}
	}
}
