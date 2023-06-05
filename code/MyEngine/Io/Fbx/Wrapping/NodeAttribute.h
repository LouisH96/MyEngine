#pragma once
#include "DataStructures/List.h"

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
				class AnimationCurveNode;
				class Model;

				class NodeAttribute
				{
				public:
					NodeAttribute() = default;
					explicit NodeAttribute(Reading::FbxObject& object);

					int64_t Id;
					std::string Type;
					Reading::FbxObject* pProperties70; //temp

					void SetParentModel(const Model& parentModel);
					void AddAnimationCurveNode(const AnimationCurveNode& animationCurveNode);

				private:
					const Model* m_pParentModel{};
					List<const AnimationCurveNode*> m_AnimationCurveNodes{};
				};
			}
		}
	}
}
