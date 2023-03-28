#pragma once

namespace MyEngine
{
	namespace Generation
	{
		class MeshUtils
		{
		public:
			static void GetIndicesToCapCircle(int nrPoints, Array<int>& indices, int arrayOffset, int valueOffset);
			static int GetNrTrianglesToCapCircle(int nrCirclePoints);
			static void AssertCapCirclePoints(int nrCirclePoints);
		};
	}
}

