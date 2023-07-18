#pragma once

namespace MyEngine
{
	class Sphere;
}

namespace MyEngine
{
	namespace Generation
	{
		class SphereGenerator
		{
		public:
			static unsigned GetNrVertices(unsigned nrCols, unsigned nrRows);
			static unsigned GetNrIndices(unsigned nrCols, unsigned nrRows);

			static void Generate(const Sphere& sphere, unsigned nrCols, unsigned nrRows, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset = {});
		};
	}
}
