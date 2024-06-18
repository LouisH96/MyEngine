#pragma once

namespace MyEngine
{
	class Hexagon;

	namespace Generation
	{
		class HexGenerator
		{
		public:
			//origin at hex center
			static void Generate(
				const Hexagon& hex, 
				const Float3& right, const Float3& normal,
				List<Rendering::V_PosNor>& vertices, List<int>& indices);

			//origin at hex center
			static void GenerateFlatRight(
				const Hexagon& hex,
				const Float3& right, const Float3& normal,
				List<Rendering::V_PosNor>& vertices, List<int>& indices);

		};
	}
}
