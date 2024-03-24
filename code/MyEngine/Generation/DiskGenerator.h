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
	}
}

#include "DiskGenerator_TriangleList.h"
#include "DiskGenerator_TriangleListIdx.h"