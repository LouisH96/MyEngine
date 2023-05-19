#pragma once
#include "TransformCurve.h"

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
		}
	}

	namespace Animation
	{
		class Joint
		{
		public:
			//---| Constructor/Destructor |---
			Joint() = default;
			Joint(const Io::Fbx::Wrapping::Model& model, const Io::Fbx::Wrapping::FbxData& fbxData);

			//---| Move/Copy |---
			Joint(const Joint& other) = delete;
			Joint(Joint&& other) noexcept;
			Joint& operator=(const Joint& other) = delete;
			Joint& operator=(Joint&& other) noexcept;

			void AddToDebugRender(float sphereSize = .05f) const;
			void AddToDebugRender(const Transform& parent, float sphereSize = .05f) const;

			void AddToDebugRender(const int64_t& time, float sphereSize = .05f) const;
			void AddToDebugRender(const int64_t& time, const Transform& parent, float sphereSize = .05f) const;

		private:
			std::string m_Name;
			Transform m_LocalTransform;
			Array<Joint> m_Children;
			Joint* m_pParent{};
			TransformCurve m_Curve;

			Float3 m_LocalTranslation;
			Quaternion m_PreRotation;
			Quaternion m_PostRotation;
		};
	}
}
