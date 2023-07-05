#pragma once
#include <cstdint>
#include <string>

#include "DataStructures/Array.h"
#include "Math/Matrices.h"

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
					~Pose() = default;

					Pose(const Pose& other) = delete;
					Pose& operator=(const Pose& other) = delete;
					Pose(Pose&& other) = default;
					Pose& operator=(Pose&& other) = default;

					//---| Fields |---
					std::string Name;
					std::string Type;
					Array<Node> Nodes;
				};
			}
		}
	}
}
