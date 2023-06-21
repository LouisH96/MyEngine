#pragma once

namespace MyEngine
{
	namespace Shapes
	{
		struct Quad
		{
			Float3 LeftBot;
			Float3 Right;
			Float3 Up;
			Float2 Size;

			const Float3& GetLeftBot() const { return LeftBot; }
			Float3 GetLeftTop() const { return LeftBot + Up * Size.y; }
			Float3 GetRightBot() const { return LeftBot + Right * Size.x; }
			Float3 GetRightTop() const { return GetLeftTop() + Right * Size.x; }
		};
	}
}
