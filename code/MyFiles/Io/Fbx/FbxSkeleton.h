#pragma once
#include "FbxJoint.h"
#include "DataStructures/Dictionary.h"

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
				explicit FbxSkeleton(const Wrapping::FbxData& fbxData, const FbxClass& fbxClass, Dictionary<uint64_t, unsigned>& modelToJoint);
				~FbxSkeleton() = default;

				//---| Move/Copy |---
				FbxSkeleton(const FbxSkeleton& other) = delete;
				FbxSkeleton(FbxSkeleton&& other) noexcept = default;
				FbxSkeleton& operator=(const FbxSkeleton& other) = delete;
				FbxSkeleton& operator=(FbxSkeleton&& other) noexcept = default;

				const Array<FbxJoint*>& GetRootJoints() const { return m_RootJoints; }
				unsigned GetNrJoints() const;

			private:
				Array<FbxJoint*> m_RootJoints;
				List<FbxJoint> m_Joints;

				void CreateJoints(const Wrapping::Model& model, const FbxClass& fbxClass, Dictionary<uint64_t, unsigned>& modelToJoint);
				void SetParentChildRelations(const Wrapping::Model& parent, const Dictionary<uint64_t, unsigned>& modelToJoint);
			};
		}
	}
}
