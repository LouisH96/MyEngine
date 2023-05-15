#pragma once

namespace MyEngine
{
	namespace Game
	{
		class Transform;
	}

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
				FbxJoint(const Wrapping::Model& model, const Wrapping::FbxData& fbxData, const Game::Transform& parentTransform);

				//---| Move/Copy |---
				FbxJoint(const FbxJoint& other) = delete;
				FbxJoint(FbxJoint&& other) noexcept;
				FbxJoint& operator=(const FbxJoint& other) = delete;
				FbxJoint& operator=(FbxJoint&& other) noexcept;

				void AddToDebugRender(float sphereSize = .05f) const;

			private:
				std::string m_Name;
				Float3 m_WorldPosition;
				Array<FbxJoint> m_Children;
				FbxJoint* m_pParent{};
			};
		}
	}
}
