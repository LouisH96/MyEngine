#pragma once
#include "ValueCurve.h"

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
		}
	}

	namespace Animation
	{
		class TransformCurve
		{
		public:
			TransformCurve() = default;
			explicit TransformCurve(const Io::Fbx::Wrapping::Model& limbNode);

			Transform AtTime(const int64_t& time) const;

		private:
			ValueCurve<float> m_TranslationCurves[3];
			ValueCurve<float> m_RotationCurves[3];
			ValueCurve<float> m_ScaleCurves[3];

			static void FromAnimationCurveNode(const Io::Fbx::Wrapping::AnimationCurveNode& node, ValueCurve<float>* pValueCurves);
			static ValueCurve<float> FromAnimationCurve(const Io::Fbx::Wrapping::AnimationCurve& curve);
		};
	}
}
