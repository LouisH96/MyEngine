#pragma once

namespace MyEngine
{
	namespace Rendering
	{
		class Gpu;

		class IDrawBatch
		{
		public:
			virtual ~IDrawBatch() = default;
			virtual void Draw(const Rendering::Gpu& gpu) = 0;
		};
	}
}