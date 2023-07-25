#pragma once

namespace MyEngine
{
	namespace Generation
	{
		class MeshUtils
		{
		public:
			static void CapCircle(int nrPoints, int arrayOffset, int valueOffset, Array<int>& indices);
			static void CapCircle(unsigned firstVertex, unsigned nrVertices, List<int>& indices);
			static int GetNrTrianglesToCapCircle(int nrCirclePoints);
			static void AssertCapCirclePoints(int nrCirclePoints);
		};
	}
}

