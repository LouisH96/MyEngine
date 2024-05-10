#pragma once
#include <Geometry\ModelTopology.h>

namespace MyEngine
{
	namespace Generation
	{
		template<ModelTopology Topology>
		class EllipsoidGenerator
		{
		public:
			struct Options
			{
				Float3 Radia;
				unsigned NrLayers; //on y-axis
				unsigned NrCorners; //on xz-plane
			};

			static unsigned GetNrVertices(const Options& options);
			static unsigned GetNrIndices(const Options& options);

			template<typename Combinator, typename Adder>
			static void GenerateVertices(Combinator combinator, Adder&& adder, const Options& options);

			template<typename Adder>
			static void GenerateIndices(unsigned firstVertex, Adder&& adder, const Options& options);
		};
	}
}
#include "EllipsoidGenerator_TriangleListIdx.h"