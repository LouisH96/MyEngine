#pragma once

namespace MyEngine
{
	namespace Generation
	{
		class TriangleGenerator
		{
		public:
			template<typename Vertex>
			static void Create_PosNor(
				const Float3& a, const Float3& b, const Float3& c,
				List<Vertex>& vertices, List<int>& indices);
		};

		template <typename Vertex>
		void TriangleGenerator::Create_PosNor(const Float3& a, const Float3& b, const Float3& c, List<Vertex>& vertices,
			List<int>& indices)
		{
			indices.EnsureIncrease(3);
			vertices.EnsureIncrease(3);
			const int first{ vertices.GetSize() };
			indices.Add(first, first + 1, first + 2);

			Vertex vertex{};
			vertex.Normal = ((b - a).Cross(c - a)).Normalized();
			vertex.Position = a;
			vertices.Add(vertex);
			vertex.Position = b;
			vertices.Add(vertex);
			vertex.Position = c;
			vertices.Add(vertex);
		}
	}
}
