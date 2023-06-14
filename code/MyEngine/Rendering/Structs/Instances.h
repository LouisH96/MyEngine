#pragma once

namespace MyEngine
{
	namespace Rendering
	{
		struct I_Pos2UvCol
		{
			Float2 posScale;
			Float2 posOffset;
			Float2 uvScale;
			Float2 uvOffset;
			Float3 color;
		};
	}
}
