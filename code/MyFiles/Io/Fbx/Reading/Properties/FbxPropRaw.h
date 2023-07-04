#pragma once
#include <istream>
#include <Io/Fbx/Reading/FbxProperty.h>

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				class FbxPropRaw
					: public FbxProperty
				{
				public:
					FbxPropRaw(std::istream& stream);

					void Print(bool compact, int nrTabs) const override;
				private:
				};
			}
		}
	}
}
