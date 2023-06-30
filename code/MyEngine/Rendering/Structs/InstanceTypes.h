#pragma once
#include "Rendering/State/InputLayout.h"

namespace MyEngine
{
	namespace Rendering
	{
		struct I_Pos2Col
		{
			Float2 offset;
			Float2 size;
			Float3 color;

			static const InputLayout::Element ELEMENTS[];
			static const int NR_ELEMENTS;

			static bool IsValid(const I_Pos2Col& inst);
			static void Invalidate(I_Pos2Col& inst);
		};
		struct I_Pos2UvCol
		{
			Float2 posOffset;
			Float2 posScale;
			Float2 uvScale;
			Float2 uvOffset;
			Float3 color;

			//combined with V_Pos2Uv
			static const InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 5;
		};
		struct I_ModelMatrices
		{
			Float4X4 model;
			Float4X4 modelViewProj;

			static const InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 2;
		};
	}
}
