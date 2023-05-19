#pragma once
#include "FbxValueCurve.h"

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

			class FbxTransformCurve
			{
			public:
				FbxTransformCurve() = default;
				explicit FbxTransformCurve(const Wrapping::Model& limbNode);

				Transform AtTime(const int64_t& time) const;

			private:
				FbxValueCurve<float> m_TranslationCurves[3];
				FbxValueCurve<float> m_RotationCurves[3];
				FbxValueCurve<float> m_ScaleCurves[3];

				static void FromAnimationCurveNode(const Wrapping::AnimationCurveNode& node, FbxValueCurve<float>* pValueCurves);
			};
		}
	}
}
