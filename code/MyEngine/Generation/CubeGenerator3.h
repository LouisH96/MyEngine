#pragma once
#include <Generation\MeshSection.h>
#include <Rendering/Mesh/MeshData.h>
#include <Geometry\Shapes\CubeAA.h>

namespace MyEngine
{
	namespace Generation
	{
		template<ModelTopology Topology, typename Cube>
		class CubeGenerator3
		{
		public:

			template<typename Combinator, typename Vertex>
			static MeshSection<Vertex, Topology> Generate(Combinator combinator, MeshData<Vertex, Topology>& meshData, const Cube& cube);

		};

		template<>
		template<typename Combinator, typename Vertex>
		inline MeshSection<Vertex, ModelTopology::LineStrip> CubeGenerator3<ModelTopology::LineStrip, CubeAA>::Generate(
			Combinator combinator,
			MeshData<Vertex, ModelTopology::LineStrip>& meshData,
			const CubeAA& cube)
		{
			MeshSection<Vertex, ModelTopology::LineStrip> section{ meshData };

			const Float3 size{ cube.GetSize() };
			Float3 pos{ cube.GetLeftBotFront() };

			meshData.Vertices.Add(combinator(pos));
			pos.y += size.y;
			meshData.Vertices.Add(combinator(pos));

			pos.x += size.x;
			meshData.Vertices.Add(combinator(pos));
			pos.y -= size.y;
			meshData.Vertices.Add(combinator(pos));
			
			pos.z += size.z;
			meshData.Vertices.Add(combinator(pos));
			pos.y += size.y;
			meshData.Vertices.Add(combinator(pos));

			pos.x -= size.x;
			meshData.Vertices.Add(combinator(pos));
			pos.y -= size.y;
			meshData.Vertices.Add(combinator(pos));

			pos.z -= size.z;
			meshData.Vertices.Add(combinator(pos));
			pos.x += size.x;
			meshData.Vertices.Add(combinator(pos));

			pos.z += size.z;
			meshData.Vertices.Add(combinator(pos));
			pos.x -= size.x;
			meshData.Vertices.Add(combinator(pos));

			pos.y += size.y;
			meshData.Vertices.Add(combinator(pos));
			pos.z -= size.z;
			meshData.Vertices.Add(combinator(pos));

			pos.x += size.x;
			meshData.Vertices.Add(combinator(pos));
			pos.z += size.z;
			meshData.Vertices.Add(combinator(pos));


			section.Finish();
			return section;
		}
	}
}