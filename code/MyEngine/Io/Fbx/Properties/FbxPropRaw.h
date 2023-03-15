#pragma once
#include <Io\Fbx\FbxProperty.h>

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			class FbxPropRaw
				: public FbxProperty
			{
			public:
				FbxPropRaw(std::istream& stream);

				void Print(int nrTabs) const override;
			private:
			};
		}
	}
}
