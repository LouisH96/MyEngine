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
				class AnimationLayer;
				class Model;
				class AnimationCurve;

				class AnimationCurveNode
				{
				public:
					AnimationCurveNode() = default;
					explicit AnimationCurveNode(Reading::FbxObject& object);

					int64_t Id;
					std::string Type;

					Reading::FbxObject* pProperties70; //temp

					void AddAnimationCurve(const AnimationCurve& animationCurve);
					void SetParentModel(const Model& model);
					void SetAnimationLayer(const AnimationLayer& animationLayer);

				private:
					Array<const AnimationCurve*> m_AnimationCurves{};
					const Model* m_pParentModel{};
					const AnimationLayer* m_pAnimationLayer{};
				};
			}
		}
	}
}
