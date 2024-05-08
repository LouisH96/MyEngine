#pragma once
#include <Generation\CubeGenerator2.h>

using namespace Generation;

template<>
inline unsigned CubeGenerator2<ModelTopology::TriangleList>::GetNrVertices(const Options& options)
{
	return 6 //faces
		* 2 //triangles
		* 3 //vertices
		;
}

template<>
inline unsigned CubeGenerator2<ModelTopology::TriangleList>::GetNrIndices(const Options& options)
{
	return 0;
}
template<>
template<typename Combinator, typename Adder>
inline void CubeGenerator2<ModelTopology::TriangleList>::GenerateVertices(Combinator combinator, Adder&& adder, const Options& options)
{
	const Float3 cubeCenter{ options.GetCenter() };
	const Float3 cubeAxis[3]
	{
		options.GetRight(),
		options.GetUp(),
		options.GetForward()
	};
	const Float3 sides[3]
	{
		options.GetRight() * (options.GetSize().x * .5f),
		options.GetUp() * (options.GetSize().y * .5f),
		options.GetForward() * (options.GetSize().z * .5f)
	};

	for (unsigned iAxis = 0; iAxis < 3; iAxis++)
		for (float iSign = 1; iSign >= -1; iSign -= 2)
		{
			const Float3 normal{ cubeAxis[iAxis] * iSign };
			const Float3 center{ cubeCenter + sides[iAxis] * iSign };

			const unsigned upAxis{ (iAxis + 1) % 3 };
			const unsigned rightAxis{ (iAxis + 2) % 3 };

			const Float3 up{ sides[upAxis] };
			const Float3 right{ sides[rightAxis] * iSign };

			//left-top triangle
			adder.Add(combinator(center - right + up, normal));
			adder.Add(combinator(center + right + up, normal));
			adder.Add(combinator(center - right - up, normal));

			//right-bot triangle
			adder.Add(combinator(center + right - up, normal));
			adder.Add(combinator(center - right - up, normal));
			adder.Add(combinator(center + right + up, normal));
		}
}

template<>
template<typename Adder>
inline void CubeGenerator2<ModelTopology::TriangleList>::GenerateIndices(Adder&& adder, const Options& options)
{
}