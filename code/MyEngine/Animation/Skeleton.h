#pragma once
#include "Joint.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Wrapping
			{
				class FbxData;
			}
		}
	}
}

namespace MyEngine
{
	namespace Animation
	{
		class Skeleton
		{
		public:
			//---| Constructor/Destructor |---
			Skeleton() = default;
			explicit Skeleton(const Io::Fbx::Wrapping::FbxData& fbxData);
			~Skeleton() = default;

			//---| Move/Copy |---
			Skeleton(const Skeleton& other) = delete;
			Skeleton(Skeleton&& other) noexcept = default;
			Skeleton& operator=(const Skeleton& other) = delete;
			Skeleton& operator=(Skeleton&& other) noexcept = default;

			void AddToDebugRenderer(float sphereSize = .05f) const;

		private:
			Joint m_RootJoint;
		};
	}
}
