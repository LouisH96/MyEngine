#pragma once

namespace MyEngine
{
	namespace Ds
	{
		template<typename>
		class Array;
	}
	namespace Math
	{
		struct Float3;
		class Cube;
	}
}

namespace MyEngine
{
	namespace Generation
	{
		class Shapes
		{
		public:
			static void GenerateCubeBuffers(const Math::Cube& cube, Ds::Array<Math::Float3>& positions, Ds::Array<Math::Float3>& normals, Ds::Array<int>& indices);

		private:

		};
	}
}
