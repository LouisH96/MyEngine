#pragma once
namespace MyEngine
{
	class Cone;

	namespace Generation
	{
		class ConeGenerator
		{
		public:
			static void Generate(
				const Cone& cone, const Float3& color, unsigned nrPoints,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset = {}
			);
		};
	}
}
