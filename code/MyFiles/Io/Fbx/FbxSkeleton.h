#pragma once
#include "FbxJoint.h"
#include "DataStructures/Dictionary.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			struct FbxLoadData;

			namespace Wrapping
			{
				class FbxData;
			}

			class FbxSkeleton
			{
			public:
				//---| Constructor/Destructor |---
				FbxSkeleton() = default;
				explicit FbxSkeleton(FbxLoadData& loadData);
				~FbxSkeleton() = default;

				//---| Move/Copy |---
				FbxSkeleton(const FbxSkeleton& other) = delete;
				FbxSkeleton(FbxSkeleton&& other) noexcept = default;
				FbxSkeleton& operator=(const FbxSkeleton& other) = delete;
				FbxSkeleton& operator=(FbxSkeleton&& other) noexcept = default;

				const List<FbxJoint>& GetJoints() const { return m_Joints; }
				FbxJoint& GetJoint(unsigned idx) { return m_Joints[idx]; }
				const Array<FbxJoint*>& GetRootJoints() const { return m_RootJoints; }
				unsigned GetNrJoints() const;

			private:
				Array<FbxJoint*> m_RootJoints;
				List<FbxJoint> m_Joints;

				void CreateJoints(const Wrapping::Model& model, FbxLoadData& loadData);
				void SetParentChildRelations(const Wrapping::Model& parent, FbxLoadData& loadData);
			};
		}
	}
}
