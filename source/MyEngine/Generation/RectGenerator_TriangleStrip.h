#pragma once

template <>
inline unsigned RectGenerator<ModelTopology::TriangleStrip>::GetNrVertices()
{
	return 4;
}

template <>
inline unsigned RectGenerator<ModelTopology::TriangleStrip>::GetNrIndices()
{
	return 0;
}

template<>
template <typename Adder>
void RectGenerator<ModelTopology::TriangleStrip>::GenerateIndices(Adder&&, unsigned)
{
}

template<>
template <typename Combinator, typename Adder>
void RectGenerator<ModelTopology::TriangleStrip>::GenerateVertices(const Combinator& combinator, Adder&& adder, const RectFloat& rect)
{
	const Float2 halfSize{ rect.GetSize() * .5f };
	adder.Add(combinator({ -halfSize.x, -halfSize.y }));
	adder.Add(combinator({ -halfSize.x, halfSize.y }));
	adder.Add(combinator({ halfSize.x, -halfSize.y }));
	adder.Add(combinator({ halfSize.x, halfSize.y }));
}
