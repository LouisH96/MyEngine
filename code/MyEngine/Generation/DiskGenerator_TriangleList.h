#pragma once
using namespace Generation;

template<>
inline unsigned DiskGenerator<ModelTopology::TriangleList>::GetNrVertices(const Options& options)
{
	return (options.NrPoints - 2) * 3;
}

template<>
inline unsigned DiskGenerator<ModelTopology::TriangleList>::GetNrIndices(const Options& options)
{
	return 0;
}

template<>
template<typename Combinator, typename Adder>
void DiskGenerator<ModelTopology::TriangleList>::Generate(Combinator combinator, Adder&& adder, const Options& options)
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
void DiskGenerator<ModelTopology::TriangleList>::GenerateIndices(unsigned firstVertexIdx, Adder&& adder, const Options& options)
{}