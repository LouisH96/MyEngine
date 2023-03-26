#pragma once
#include "App/Wrappers/Dx/InputLayout.h"
#include "Math/Float2.h"
#include "Math/Float3.h"

namespace MyEngine
{
	namespace Rendering
	{
		struct V_PosCol
		{
			Math::Float3 Pos{};
			Math::Float3 Color{};

			static const Dx::InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 2;
		};
		struct V_PosColNorm
		{
			Math::Float3 Pos{};
			Math::Float3 Color{};
			Math::Float3 Normal{};

			static const Dx::InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 3;
		};
		struct V_PosUv
		{
			Math::Float3 Pos{};
			Math::Float2 Uv{};

			static const Dx::InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 2;
		};
		struct V_PosNorUv
		{
			Math::Float3 Pos{};
			Math::Float3 Normal{};
			Math::Float2 Uv{};

			static const Dx::InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 3;
		};
	}
}
