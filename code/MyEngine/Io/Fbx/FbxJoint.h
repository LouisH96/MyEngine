#pragma once
#include "FbxTransformCurve.h"

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
				FbxJoint(const Wrapping::Model& model, const Wrapping::FbxData& fbxData, const FbxClass& fbxClass);

				//---| Move/Copy |---
				FbxJoint(const FbxJoint& other) = delete;
				FbxJoint(FbxJoint&& other) noexcept;
				FbxJoint& operator=(const FbxJoint& other) = delete;
				FbxJoint& operator=(FbxJoint&& other) noexcept;

				void AddToDebugRender(float sphereSize = .05f) const;
				void AddToDebugRender(const Transform& parent, float sphereSize = .05f) const;

				void AddToDebugRender(const FbxAnimationLayer& layer, const int64_t& time, float scale = 1, float sphereSize = .05f) const;
				void AddToDebugRender(const FbxAnimationLayer& layer, const int64_t& time, const Transform& parent, float scale = 1, float sphereSize = .05f) const;

			private:
				std::string m_Name;
				Transform m_LocalTransform;
				Array<FbxJoint> m_Children;
				FbxJoint* m_pParent{};

				Array<FbxTransformCurve> m_Curves;

				Float3 m_LocalTranslation;
				Quaternion m_PreRotation;
				Quaternion m_PostRotation;

				const FbxTransformCurve* FindCurve(const FbxAnimationLayer& layer) const;
			};
		}
	}
}
