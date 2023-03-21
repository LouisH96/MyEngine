#pragma once

namespace MyEngine
{
	namespace Generation
	{
		class MeshUtils
		{
		public:

			static void MakePointAndNormalBuffersFromFbxIndices(const Array<int>& indices, const Array<Math::Float3>& points, 
				Array<Math::Float3>& outputPoints, Array<Math::Float3>& outputNormals);
		};
	}
}

