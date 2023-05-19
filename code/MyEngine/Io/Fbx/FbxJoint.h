#pragma once
#include "FbxTransformCurve.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
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
				FbxJoint(const Wrapping::Model& model, const Wrapping::FbxData& fbxData);

				//---| Move/Copy |---
				FbxJoint(const FbxJoint& other) = delete;
				FbxJoint(FbxJoint&& other) noexcept;
				FbxJoint& operator=(const FbxJoint& other) = delete;
				FbxJoint& operator=(FbxJoint&& other) noexcept;

				void AddToDebugRender(float sphereSize = .05f) const;
				void AddToDebugRender(const Transform& parent, float sphereSize = .05f) const;

				void AddToDebugRender(const int64_t& time, float sphereSize = .05f) const;
				void AddToDebugRender(const int64_t& time, const Transform& parent, float sphereSize = .05f) const;

			private:
				std::string m_Name;
				Transform m_LocalTransform;
				Array<FbxJoint> m_Children;
				FbxJoint* m_pParent{};
				FbxTransformCurve m_Curve;

				Float3 m_LocalTranslation;
				Quaternion m_PreRotation;
				Quaternion m_PostRotation;
			};
		}
	}
}
