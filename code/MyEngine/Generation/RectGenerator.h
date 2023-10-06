#pragma once
#include "Geometry/Shapes/Rects.h"
#include "Geometry/ModelTopology.h"

namespace MyEngine
{
	template<ModelTopology Topology>
	class RectGenerator
	{
	public:
		static unsigned GetNrVertices();
		static unsigned GetNrIndices();
		template<typename Adder>
		static void GenerateIndices(Adder&& adder, unsigned offset);
		template<typename Combinator, typename Adder>
		static void GenerateVertices(const Combinator& combinator, Adder&& adder, const RectFloat& rect);
	};

	template <>
	inline unsigned RectGenerator<ModelTopology::TriangleListIdx>::GetNrVertices()
	{
		return 4;
	}

	template <>
	inline unsigned RectGenerator<ModelTopology::TriangleListIdx>::GetNrIndices()
	{
		return 2 * 3;
	}

	template<>
	template<typename Adder>
	void RectGenerator<ModelTopology::TriangleListIdx>::GenerateIndices(Adder&& adder, unsigned offset)
	{
		adder.Add(0 + offset, 1 + offset, 2 + offset);
		adder.Add(0 + offset, 2 + offset, 3 + offset);
	}

	template<>
	template<typename Combinator, typename Adder>
	void RectGenerator<ModelTopology::TriangleListIdx>::GenerateVertices(const Combinator& combinator, Adder&& adder, const RectFloat& rect)
	{
		adder.Add(combinator(rect.GetLeftBot()));
		adder.Add(combinator(rect.GetLeftTop()));
		adder.Add(combinator(rect.GetRightTop()));
		adder.Add(combinator(rect.GetRightBot()));
	}
}

#include "RectGenerator_TriangleStrip.h"
