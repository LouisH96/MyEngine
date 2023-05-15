#pragma once

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				class FbxObject;
			}

			namespace Wrapping
			{
				class Deformer
				{
				public:
					Deformer() = default;
					Deformer(Reading::FbxObject& object);

					int64_t Id;
				};
			}
		}
	}
}
