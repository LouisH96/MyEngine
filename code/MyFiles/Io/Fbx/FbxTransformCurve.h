#pragma once
#include "FbxValueCurve.h"
#include "Transform/Transform.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Wrapping
			{
				class AnimationCurveNode;
				class AnimationCurve;
				class Model;
			}
			class FbxAnimationLayer;

			class FbxTransformCurve
			{
			public:
				FbxTransformCurve() = default;
				explicit FbxTransformCurve(const Wrapping::Model& limbNode, const FbxAnimationLayer& layer);

				Game::Transform AtTime(const uint64_t& time) const;
				bool IsInLayer(const FbxAnimationLayer& layer) const;

			private:
				const FbxAnimationLayer* m_pLayer;
				FbxValueCurve<float> m_TranslationCurves[3];
				FbxValueCurve<float> m_RotationCurves[3];
				FbxValueCurve<float> m_ScaleCurves[3];

				static void FromAnimationCurveNode(const Wrapping::AnimationCurveNode& node, FbxValueCurve<float>* pValueCurves);
				static void FromDefaultValue(const Double3& value, FbxValueCurve<float>* pValueCurves);
				static void FromDefaultValue(const Float3& value, FbxValueCurve<float>* pValueCurves);
			};
		}
	}
}
