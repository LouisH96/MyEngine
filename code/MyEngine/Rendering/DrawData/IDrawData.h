#pragma once

namespace MyEngine
{
	namespace Rendering
	{
		class Gpu;

		class IDrawData
		{
		public:
			virtual ~IDrawData() = default;
			virtual void Draw(const Rendering::Gpu& gpu) = 0;
		};
	}
}