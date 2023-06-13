#pragma once

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				class FbxObject;
			}

			namespace Wrapping
			{
				struct Pose
				{
					//---| Types |---
					struct Node
					{
						constexpr static int64_t INVALID_ID = -1;

						int64_t Id{ INVALID_ID };
						Double4X4 Matrix{};

						bool IsValid() const { return Id != INVALID_ID; }
					};

					//---| Constructor |---
					Pose() = default;
					explicit Pose(Reading::FbxObject& poseObject);

					//---| Fields |---
					std::string Name;
					std::string Type;
					Array<Node> Nodes;
				};
			}
		}
	}
}
