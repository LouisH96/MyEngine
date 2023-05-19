#pragma once
#include <Rendering/State/InputLayout.h>
#include "Math/Float2.h"
#include "Math/Float3.h"

namespace MyEngine
{
	namespace Rendering
	{
		struct V_Pos2Uv
		{
			Math::Float2 pos;
			Math::Float2 uv;

			static const InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 2;
		};
		struct V_PosNor
		{
			Float3 Position{};
			Float3 Normal{};
			static const InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 2;
		};
		struct V_PosCol
		{
			Math::Float3 Pos{};
			Math::Float3 Color{};

			static const InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 2;
		};
		struct V_PosColNorm
		{
			Math::Float3 Pos{};
			Math::Float3 Color{};
			Math::Float3 Normal{};

			static const InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 3;
		};
		struct V_PosNorCol
		{
			Math::Float3 Pos{};
			Math::Float3 Normal{};
			Math::Float3 Color{};

			static const InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 3;
		};
		struct V_PosUv
		{
			Math::Float3 Pos{};
			Math::Float2 Uv{};

			static const InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 2;
		};
		struct V_PosNorUv
		{
			Math::Float3 Pos{};
			Math::Float3 Normal{};
			Math::Float2 Uv{};

			static const InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 3;
		};
	}
}
