#pragma once
#include "Game/Transform.h"

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
				void AddToDebugRender(const Game::Transform& parent, float sphereSize = .05f) const;

			private:
				std::string m_Name;
				Game::Transform m_LocalTransform;
				Array<FbxJoint> m_Children;
				FbxJoint* m_pParent{};
			};
		}
	}
}
