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
			static constexpr unsigned NR_ELEMENTS{ 3 };

			bool IsValid() const;
			void Invalidate();

			Float2& GetCenter() { return offset; }
			Float2& GetSize() { return size; }

			const Float2& GetCenter() const { return offset; }
			const Float2& GetSize() const { return size; }
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
			static constexpr unsigned NR_ELEMENTS{ 5 };

			bool IsValid() const;
			void Invalidate();

			Float2& GetCenter() { return posOffset; }
			Float2& GetSize() { return posScale; }

			const Float2& GetCenter() const { return posOffset; }
			const Float2& GetSize() const { return posScale; }
		};
		struct I_ModelMatrix
		{
			Float4X4 model;

			static const InputLayout::Element ELEMENTS[];
			static constexpr unsigned NR_ELEMENTS{ 1 };
		};
		struct I_ModelMatrices
		{
			Float4X4 model;
			Float4X4 modelViewProj;

			static const InputLayout::Element ELEMENTS[];
			static constexpr unsigned NR_ELEMENTS{ 2 };
		};
		struct I_PosCol
		{
			Float3 pos;
			Float3 col;

			static const InputLayout::Element ELEMENTS[];
			static constexpr unsigned NR_ELEMENTS{ 2 };

			bool IsValid() const;
			void Invalidate();
		};
	}
}
