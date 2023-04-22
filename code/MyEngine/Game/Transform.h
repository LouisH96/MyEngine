#pragma once
#include "Math/Quaternion.h"

namespace MyEngine
{
	namespace Game
	{
		class Transform
		{
		public:
			Math::Float3 Position{};
			Math::Quaternion Rotation{};
		};
	}
}
