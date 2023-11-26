#pragma once
#include "FbxTransformCurve.h"
#include "Math/Quaternion.h"
#include "Math/Vectors.h"
#include "Transform/Transform.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			class FbxClass;

			namespace Wrapping
			{
				class FbxData;
				class Model;
			}

			class FbxJoint
			{
			public:
				//---| Constructor/Destructor |---
				FbxJoint() = default;
				FbxJoint(const Wrapping::Model& model, const FbxClass& fbxClass);

				//---| Move/Copy |---
				FbxJoint(const FbxJoint& other) = delete;
				FbxJoint(FbxJoint&& other) noexcept;
				FbxJoint& operator=(const FbxJoint& other) = delete;
				FbxJoint& operator=(FbxJoint&& other) noexcept;

				//---| Functions |---
				const FbxTransformCurve* FindCurve(const FbxAnimationLayer& layer) const;

				const Quaternion& GetPreRotation() const { return m_PreRotation; }
				const Quaternion& GetPostRotation() const { return m_PostRotation; }

				void SetChildren(Array<FbxJoint*>&& children) { m_Children = children; }
				const Array<FbxJoint*>& GetChildren() const { return m_Children; }

				void SetParent(FbxJoint* pParent) { m_pParent = pParent; }
				bool HasParent() const { return m_pParent; }
				const FbxJoint& GetParent() const { return *m_pParent; }

				const Float4X4& GetBoneTransform() const { return m_BoneTransform; }
				void CalculateBoneTransforms();

			private:
				std::string m_Name;
				Game::Transform m_LocalTransform;
				Float4X4 m_BoneTransform; //transform of bone in model-space
				Array<FbxJoint*> m_Children;
				FbxJoint* m_pParent{};

				Array<FbxTransformCurve> m_Curves;

				Float3 m_LocalTranslation;
				Quaternion m_PreRotation;
				Quaternion m_PostRotation;

			};
		}
	}
}
