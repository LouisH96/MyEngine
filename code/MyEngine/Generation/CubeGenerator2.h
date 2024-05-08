#pragma once
#include "Geometry/ModelTopology.h"
#include "Geometry\Shapes\Cube.h"

namespace MyEngine
{
	namespace Generation
	{
		template<ModelTopology Topology>
		class CubeGenerator2
		{
		public:
			using Options = Cube;

			static unsigned GetNrVertices(const Options& options);
			static unsigned GetNrIndices(const Options& options);

			template<typename Combinator, typename Adder>
			static void GenerateVertices(Combinator combinator, Adder&& adder, const Options& options);

			template<typename Adder>
			static void GenerateIndices(Adder&& adder, const Options& options);

		private:
		};
	}
}

#include "CubeGenerator2_TriangleList.h"
