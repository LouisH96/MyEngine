#pragma once
#include "FbxValueCurve.h"
#include "Math/Double3.h"

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
				static void FromDefaultValue(const Double3& value, FbxValueCurve<float>* pValueCurves);
				static void FromDefaultValue(const Float3& value, FbxValueCurve<float>* pValueCurves);
			};
		}
	}
}
