#pragma once
#include <Generation\EllipsoidGenerator.h>

using namespace Generation;

template<>
inline unsigned EllipsoidGenerator<ModelTopology::TriangleListIdx>::GetNrVertices(const Options& options)
{
	return 2
		+ options.NrLayers * options.NrCorners;
}
template<>
inline unsigned EllipsoidGenerator<ModelTopology::TriangleListIdx>::GetNrIndices(const Options& options)
{
	return options.NrCorners * 2 * 3 * options.NrLayers;
}
template<>
template<typename Combinator, typename Adder>
inline void EllipsoidGenerator<ModelTopology::TriangleListIdx>::GenerateVertices(Combinator combinator, Adder&& adder, const Options& options)
{
	adder.Add(combinator({ 0, -options.Radia[1], 0 }, { 0,-1,0 }));

	const float invRadia[3]
	{
		1.f / options.Radia[0],
		1.f / options.Radia[1],
		1.f / options.Radia[2]
	};

	const float sliceAngleStep{ Constants::PI / (options.NrLayers + 1) };
	const float cornerAngleStep{ Constants::PI * 2 / options.NrCorners };

	for (unsigned iSlice = 0; iSlice < options.NrLayers; iSlice++)
	{
		const float sliceAngle{ sliceAngleStep * (iSlice + 1) };
		const float sliceAngleCos{ cos(sliceAngle) };
		const float sliceAngleSin{ sin(sliceAngle) };

		const float sliceHeight{ -sliceAngleCos * options.Radia[1] };
		const float sliceRadius{ sliceAngleSin * options.Radia[0] };

		Float3 point{ 0,sliceHeight, 0 };

		for (unsigned iCorner = 0; iCorner < options.NrCorners; iCorner++)
		{
			const float cornerAngle{ iCorner * cornerAngleStep };
			const float cornerAngleCos{ cos(cornerAngle) };
			const float cornerAngleSin{ sin(cornerAngle) };

			point.x = cornerAngleCos * sliceAngleSin * options.Radia[0];
			point.z = cornerAngleSin * sliceAngleSin * options.Radia[2];

			const Float3 normal{ Float3{
				cornerAngleCos * sliceAngleSin * invRadia[0],
				-sliceAngleCos * invRadia[1] ,
				cornerAngleSin * sliceAngleSin * invRadia[2]}.Normalized() };

			adder.Add(combinator(point, normal));
		}
	}

	adder.Add(combinator({ 0,options.Radia[1], 0 }, { 0,1,0 }));
}

template<>
template<typename Adder>
inline void EllipsoidGenerator<ModelTopology::TriangleListIdx>::GenerateIndices(unsigned firstVertex, Adder&& adder, const Options& options)
{
	unsigned iVertex{ firstVertex };

	const unsigned bottomVertex{ iVertex };

	//bottom cap
	adder.Add(iVertex++);
	for (unsigned iCorner = 0; iCorner < options.NrCorners - 1; iCorner++)
	{
		adder.Add(iVertex++);
		adder.Add(iVertex);
		adder.Add(bottomVertex);
	}
	adder.Add(iVertex);
	iVertex = bottomVertex + 1;
	adder.Add(iVertex);

	//middle layers
	for (unsigned iLayer = 0; iLayer + 1 < options.NrLayers; iLayer++)
	{
		const unsigned firstLayerCorner{ iVertex };

		for (unsigned iCorner = 0; iCorner + 1 < options.NrCorners; iCorner++)
		{
			const unsigned leftBottom{ iVertex++ };
			const unsigned rightBottom{ leftBottom + 1 };
			const unsigned leftTop{ leftBottom + options.NrCorners };
			const unsigned rightTop{ leftTop + 1 };

			adder.Add(leftBottom, leftTop, rightTop);
			adder.Add(rightTop, rightBottom, leftBottom);
		}

		const unsigned leftBot{ iVertex++ };
		const unsigned leftTop{ leftBot + options.NrCorners };
		const unsigned rightBot{ firstLayerCorner };
		const unsigned rightTop{ iVertex };

		adder.Add(leftBot, leftTop, rightTop);
		adder.Add(rightTop, rightBot, leftBot);
	}

	//top cap
	const unsigned firstTopCapVertex{ iVertex };
	const unsigned topVertex{ firstTopCapVertex + options.NrCorners };

	for (unsigned iCorner = 0; iCorner + 1 < options.NrCorners; iCorner++)
	{
		adder.Add(iVertex);
		adder.Add(topVertex);
		adder.Add(++iVertex);
	}

	adder.Add(iVertex);
	adder.Add(topVertex);
	adder.Add(firstTopCapVertex);
}