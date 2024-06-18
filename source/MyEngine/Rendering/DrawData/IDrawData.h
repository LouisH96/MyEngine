#pragma once

namespace MyEngine
{
	namespace Rendering
	{
		class IDrawData
		{
		public:
			virtual ~IDrawData() = default;
			virtual void Draw() = 0;
		};
	}
}