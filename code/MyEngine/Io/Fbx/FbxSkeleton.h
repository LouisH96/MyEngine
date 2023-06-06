#pragma once
#include "FbxJoint.h"

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

			class FbxSkeleton
			{
			public:
				//---| Constructor/Destructor |---
				FbxSkeleton() = default;
				explicit FbxSkeleton(const Wrapping::FbxData& fbxData, const FbxClass& fbxClass);
				~FbxSkeleton() = default;

				//---| Move/Copy |---
				FbxSkeleton(const FbxSkeleton& other) = delete;
				FbxSkeleton(FbxSkeleton&& other) noexcept = default;
				FbxSkeleton& operator=(const FbxSkeleton& other) = delete;
				FbxSkeleton& operator=(FbxSkeleton&& other) noexcept = default;

				void AddToDebugRenderer(const FbxAnimationLayer& layer, float scale = 1,  float sphereSize = .005f) const;
				void AddToDebugRenderer(const FbxAnimationLayer& layer, const int64_t& time, float scale = 1, float sphereSize = .005f) const;

			private:
				FbxJoint m_RootJoint;
			};
		}
	}
}
