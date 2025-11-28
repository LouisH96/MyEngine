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
			static const unsigned NR_ELEMENTS;

			static Array<V_Pos2> GetCenterRect();

			bool IsValid() const { return pos.x != Float::Max(); }
			void Invalidate() { pos.x = Float::Max(); }
		};
		struct V_Pos2Col
		{
			Float2 pos;
			Float3 col;

			static const InputLayout::Element ELEMENTS[];
			static constexpr unsigned NR_ELEMENTS = 2;

			bool IsValid() const { return pos.x != Float::INVALID; }
			void Invalidate() { pos.x = Float::INVALID; }
		};
		struct V_Pos2Uv
		{
			Float2 pos;
			Float2 uv;

			static const InputLayout::Element ELEMENTS[];
			static constexpr unsigned NR_ELEMENTS = 2;

			static Array<V_Pos2Uv> GetCenterRect();
		};
		struct V_Pos2ColUv
		{
			Float2 pos;
			Float3 col;
			Float2 uv;

			static const InputLayout::Element ELEMENTS[];
			static constexpr unsigned NR_ELEMENTS = 3;

			bool IsValid() const { return pos.x != Float::MAX; }
			void Invalidate() { pos.x = Float::MAX; }
		};

		//---| 3D POSITION |---
		struct V_PosNor
		{
			Float3 Position{};
			Float3 Normal{};
			static const InputLayout::Element ELEMENTS[];
			static constexpr unsigned NR_ELEMENTS = 2;
		};
		struct V_PosCol
		{
			Float3 Pos{};
			Float3 Color{};

			static const InputLayout::Element ELEMENTS[];
			static constexpr unsigned NR_ELEMENTS = 2;

			bool IsValid() const { return Pos.x != Float::MAX; }
			void Invalidate() { Pos.x = Float::MAX; }
		};
		struct V_PosColNorm
		{
			Float3 Pos{};
			Float3 Color{};
			Float3 Normal{};

			static const InputLayout::Element ELEMENTS[];
			static constexpr unsigned NR_ELEMENTS = 3;
		};
		struct V_PosNorCol
		{
			Float3 Pos{};
			Float3 Normal{};
			Float3 Color{};

			static const InputLayout::Element ELEMENTS[];
			static constexpr unsigned NR_ELEMENTS = 3;
		};
		struct V_PosUv
		{
			Float3 Pos{};
			Float2 Uv{};

			static const InputLayout::Element ELEMENTS[];
			static constexpr unsigned NR_ELEMENTS = 2;
		};
		struct V_PosNorUv
		{
			Float3 Pos{};
			Float3 Normal{};
			Float2 Uv{};

			static const InputLayout::Element ELEMENTS[];
			static constexpr unsigned NR_ELEMENTS = 3;
		};
		struct V_PosNorUvSkin
		{
			Float3 Pos{};
			Float3 Normal{};
			Float2 Uv{};
			Int4 BoneIds{};
			Float4 BoneWeights{};

			static const InputLayout::Element ELEMENTS[];
			static constexpr unsigned NR_ELEMENTS = 5;
		};
	}
}
