#pragma once

template <>
inline unsigned Generation::DiskGenerator<ModelTopology::TriangleListIdx>::GetNrVertices(const Options& options)
{
	return options.NrPoints;
}

template <>
inline unsigned Generation::DiskGenerator<ModelTopology::TriangleListIdx>::GetNrIndices(const Options& options)
{
	return (options.NrPoints - 2) * 3;
}

template<>
template <typename Combinator, typename Adder>
void Generation::DiskGenerator<ModelTopology::TriangleListIdx>::Generate(Combinator combinator, Adder&& adder, const Options& options)
{
	const float radius{ options.Radius };
	const float angleStep{ Constants::PI2 / static_cast<float>(options.NrPoints) };

	for (unsigned i = 0; i < options.NrPoints; i++)
	{
		const float angle{ angleStep * static_cast<float>(i) };
		const Float2 point{ cosf(angle) * radius, sinf(angle) * radius };
		adder.Add(combinator(point));
	}
}

template<>
template <typename Adder>
void Generation::DiskGenerator<ModelTopology::TriangleListIdx>::GenerateIndices(unsigned firstVertexIdx, Adder&& adder, const Options& options)
{
	int i0 = 0;
	int i1 = 1;
	int i2 = 2;
	while (i2 != i0)
	{
		//first triangle
		adder.Add(i0 + firstVertexIdx);
		adder.Add(i2 + firstVertexIdx);
		adder.Add(i1 + firstVertexIdx);

		//second triangle
		i1 = i2;
		i2 = i0 - 1;
		if (i2 < 0) i2 += GetNrVertices(options);
		if (i1 == i2) return;

		adder.Add(i0 + firstVertexIdx);
		adder.Add(i2 + firstVertexIdx);
		adder.Add(i1 + firstVertexIdx);

		//find next
		i0 = i2;
		i2 = i1 + 1;
	}
}