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

				const Array<FbxJoint>& GetRootJoints() const { return m_RootJoints; }

			private:
				Array<FbxJoint> m_RootJoints;
			};
		}
	}
}
