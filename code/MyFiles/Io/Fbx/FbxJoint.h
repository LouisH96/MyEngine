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
			struct FbxLoadData;
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
				FbxJoint(const Wrapping::Model& model, FbxLoadData& loadData);

				//---| Move/Copy |---
				FbxJoint(const FbxJoint& other) = delete;
				FbxJoint(FbxJoint&& other) noexcept;
				FbxJoint& operator=(const FbxJoint& other) = delete;
				FbxJoint& operator=(FbxJoint&& other) noexcept;

				//---| Functions |---
				const FbxTransformCurve* FindCurve(const FbxAnimationLayer& layer) const;

				const Quaternion& GetPreRotation() const { return m_PreRotation; }
				const Quaternion& GetPostRotation() const { return m_PostRotation; }
				const Float3& GetTranslation() const { return m_Translation; }

				void SetChildren(Array<FbxJoint*>&& children) { m_Children = children; }
				const Array<FbxJoint*>& GetChildren() const { return m_Children; }

				void SetParent(FbxJoint* pParent) { m_pParent = pParent; }
				bool HasParent() const { return m_pParent; }
				const FbxJoint& GetParent() const { return *m_pParent; }

				const Game::Transform& GetLocalTransform() const { return m_LocalTransform; }
				const Game::Transform& GetBoneTransform() const { return m_BoneTransform; }
				void CalculateBoneTransforms();

				void PrintLocalData() const;
				const std::string& GetName() const { return m_Name; }

				const Float3& GetPreRotationEulers() const { return m_PreRotationEulers; }
				const Float3& GetPostRotationEulers() const { return m_PostRotationEulers; }
				const Float3& GetLclRotationEulers() const { return m_LclRotationEulers; }

			private:
				std::string m_Name;
				Game::Transform m_LocalTransform;
				Game::Transform m_BoneTransform; //transform of bone in model-space
				Array<FbxJoint*> m_Children;
				FbxJoint* m_pParent{};

				Array<FbxTransformCurve> m_Curves;

				Float3 m_Translation;
				Quaternion m_PreRotation;
				Quaternion m_PostRotation;

				Float3 m_LclRotationEulers;
				Float3 m_PreRotationEulers;
				Float3 m_PostRotationEulers;
			};
		}
	}
}
