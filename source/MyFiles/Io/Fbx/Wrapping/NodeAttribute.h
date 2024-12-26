#pragma once
#include <cstdint>
#include <string>

#include "DataStructures/List.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				class FbxElement;
			}

			namespace Wrapping
			{
				class AnimationCurveNode;
				class Model;

				class NodeAttribute
				{
				public:
					NodeAttribute() = default;
					explicit NodeAttribute(Reading::FbxElement& object);

					int64_t Id{};
					std::string Type;
					Reading::FbxElement* pProperties70{}; //temp

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
