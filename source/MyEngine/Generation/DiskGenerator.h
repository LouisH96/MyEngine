#pragma once
#include "Geometry/ModelTopology.h"

namespace MyEngine
{
namespace Generation
{
template<ModelTopology Topology>
class DiskGenerator
{
public:
	struct Options
	{
		float Radius = 1;
		unsigned NrPoints = 32;
	};

	static unsigned GetNrVertices(const Options& options);
	static unsigned GetNrIndices(const Options& options);

	template<typename Combinator, typename Adder>
	static void Generate(Combinator combinator, Adder&& adder, const Options& options);

	template<typename Adder>
	static void GenerateIndices(unsigned firstVertexIdx, Adder&& adder, const Options& options);
};

template<>
inline unsigned MyEngine::Generation::DiskGenerator<ModelTopology::TriangleList>::GetNrVertices(const Options& options)
{
	return (options.NrPoints - 2) * 3;
}

template<>
inline unsigned Generation::DiskGenerator<ModelTopology::TriangleList>::GetNrIndices(const Options&)
{
	return 0;
}

template<>
template<typename Combinator, typename Adder>
void Generation::DiskGenerator<ModelTopology::TriangleList>::Generate(Combinator combinator, Adder&& adder, const Options& options)
{
	const float step{ Constants::PI2 / options.NrPoints };

	//first triangle
	adder.Add(combinator(Float2{ options.Radius, 0 }));
	Float2 lastTop{ cosf(step) * options.Radius, sinf(step) * options.Radius };
	Float2 lastBot{ lastTop.x, -lastTop.y };
	adder.Add(combinator(lastBot));
	adder.Add(combinator(lastTop));

	//middle triangles
	for (unsigned i = 2; i < options.NrPoints / 2; i += 1)
	{
		const float angle{ i * step };
		const Float2 newTop{ cosf(angle) * options.Radius, sinf(angle) * options.Radius };
		const Float2 newBot{ newTop.x, -newTop.y };

		adder.Add(combinator(lastTop));
		adder.Add(combinator(lastBot));
		adder.Add(combinator(newBot));

		adder.Add(combinator(lastTop));
		adder.Add(combinator(newBot));
		adder.Add(combinator(newTop));

		lastTop = newTop;
		lastBot = newBot;
	}

	//end triangle
	if (options.NrPoints % 2 == 0)
	{
		adder.Add(combinator(lastTop));
		adder.Add(combinator(lastBot));
		adder.Add(combinator({ -options.Radius, 0 }));
	}
}

template<>
template<typename Adder>
void Generation::DiskGenerator<ModelTopology::TriangleList>::GenerateIndices(unsigned firstVertexIdx, Adder&& adder, const Options& options)
{
}

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
}
}