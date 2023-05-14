#pragma once
#include <cstdint>
#include <string>

#include "Io/Fbx/Reading/Properties70.h"

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
				class AnimationCurveNode
				{
				public:
					AnimationCurveNode() = default;
					explicit AnimationCurveNode(Reading::FbxObject& object);

					int64_t Id;
					std::string Type;

					Reading::FbxObject* pProperties70; //temp
				};
			}
		}
	}
}
