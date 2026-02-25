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
				FbxJoint(const Wrapping::Model& model, const FbxLoadData& loadData, unsigned id);

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

				const Transform& GetLocalTransform() const { return m_LocalTransform; }
				const Float4X4& GetBindTransform() const { return m_BindTransform; }
				Float4X4 CalculatePoseMatrix() const;

				void PrintLocalData() const;
				const std::string& GetName() const { return m_Name; }
				unsigned GetId() const { return m_Id; }

				const Float3& GetPreRotationEulers() const { return m_PreRotationEulers; }
				const Float3& GetPostRotationEulers() const { return m_PostRotationEulers; }
				const Float3& GetLclRotationEulers() const { return m_LclRotationEulers; }

				const Transform& GetPreRotationZYX() const { return m_PreRotationTransform; }
				const Transform& GetPostRotationXYZ() const { return m_PostRotationTransform; }

				void CalculateBindTransforms();

			private:
				std::string m_Name;
				unsigned m_Id{};
				Transform m_LocalTransform;
				Transform m_PreRotationTransform;
				Transform m_PostRotationTransform;

				Float4X4 m_BindTransform;
				Array<FbxJoint*> m_Children;
				FbxJoint* m_pParent{};

				Array<FbxTransformCurve> m_Curves;

				Float3 m_Translation;
				Quaternion m_PreRotation;
				Quaternion m_PostRotation;

				Float3 m_LclRotationEulers;
				Float3 m_PreRotationEulers;
				Float3 m_PostRotationEulers;

				bool HasValidBindTransform() const;
				void SetInvalidBindTransform();
			};
		}
	}
}
