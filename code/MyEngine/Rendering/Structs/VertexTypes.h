#pragma once
#include <Rendering/State/InputLayout.h>

namespace MyEngine
{
	namespace Rendering
	{
		//---| 2D POSITION  |---
		struct V_Pos2
		{
			Float2 pos;

			static const InputLayout::Element ELEMENTS[];
			static const int NR_ELEMENTS;
		};
		struct V_Pos2Col
		{
			Float2 pos;
			Float3 col;

			static const InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 2;
		};
		struct V_Pos2Uv
		{
			Float2 pos;
			Float2 uv;

			static const InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 2;
		};

		//---| 3D POSITION |---
		struct V_PosNor
		{
			Float3 Position{};
			Float3 Normal{};
			static const InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 2;
		};
		struct V_PosCol
		{
			Float3 Pos{};
			Float3 Color{};

			static const InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 2;
		};
		struct V_PosColNorm
		{
			Float3 Pos{};
			Float3 Color{};
			Float3 Normal{};

			static const InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 3;
		};
		struct V_PosNorCol
		{
			Float3 Pos{};
			Float3 Normal{};
			Float3 Color{};

			static const InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 3;
		};
		struct V_PosUv
		{
			Float3 Pos{};
			Float2 Uv{};

			static const InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 2;
		};
		struct V_PosNorUv
		{
			Float3 Pos{};
			Float3 Normal{};
			Float2 Uv{};

			static const InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 3;
		};
	}
}
