#pragma once
#include "DataStructures/SortedList.h"

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
		class Joint;

		class JointCurve
		{
		public:
			JointCurve() = default;
			explicit JointCurve(const Io::Fbx::Wrapping::Model& model);

		private:
			Array<int64_t> m_Times{};
			Array<Transform> m_Transforms{};

			static void AddTimes(SortedList<int64_t>& list, const Io::Fbx::Wrapping::AnimationCurveNode& node);
			static void AddTimes(SortedList<int64_t>& list, const Io::Fbx::Wrapping::AnimationCurve& curve);
		};
	}
}
