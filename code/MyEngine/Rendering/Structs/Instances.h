#pragma once
#include "Rendering/State/InputLayout.h"

namespace MyEngine
{
	namespace Rendering
	{
		struct I_Pos2UvCol
		{
			Float2 posOffset;
			Float2 posScale;
			Float2 uvScale;
			Float2 uvOffset;
			Float3 color;

			//combined with V_Pos2Uv
			static const InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 5 + 2;
		};
		struct I_ModelMatrices
		{
			Float4X4 model;
			Float4X4 modelViewProj;

			static const InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 2 * 4 + 3;
		};
	}
}
