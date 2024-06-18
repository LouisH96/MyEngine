#pragma once
#include <Geometry\Shapes\CubeAA.h>
#include <Generation\MeshSection.h>
#include <Generation\StripLoopCorneredGenerator.h>
#include <Rendering\Mesh\MeshData.h>

namespace MyEngine
{
	namespace Generation
	{
		//--| Classes |--
		template<ModelTopology Topology, typename Cube>
		class CubeGenerator3
		{
		public:
			template<typename Combinator, typename Vertex>
			static MeshSection<Vertex, Topology> Generate(
				Combinator combinator, MeshData<Vertex, Topology>& meshData, const Cube& cube);
		};

		class CubeGenerator3Helper
		{
		public:
			template<typename Cube>
			static Float3 GetRight(const Cube& cube);

			template<typename Cube>
			static Float3 GetUp(const Cube& cube);

			template<typename Cube>
			static Float3 GetForward(const Cube& cube);
		};

		//--| Implementations| |--
		template<ModelTopology Topology, typename Cube>
		template<typename Combinator, typename Vertex>
		inline MeshSection<Vertex, Topology> CubeGenerator3<Topology, Cube>::Generate(
			Combinator combinator, MeshData<Vertex, Topology>& meshData, const Cube& cube)
		{
			//Create Section
			MeshSection<Vertex, Topology> section{ meshData };

			//Create StripLoopGenerator
			using Generator = StripLoopCorneredGenerator<Vertex, Topology>;
			using Options = StripLoopCorneredGeneratorBase::Options;

			Options options{};
			options.NrCorners = 4;

			Generator generator{ meshData, options };

			//Add Vertices
			const Float3 right{ CubeGenerator3Helper::GetRight(cube) };
			const Float3 up{ CubeGenerator3Helper::GetUp(cube) };
			const Float3 forward{ CubeGenerator3Helper::GetForward(cube) };

			const Float3 rightSize{ right * cube.GetWidth() };
			const Float3 upSize{ up * cube.GetHeight() };
			const Float3 forwardSize{ forward * cube.GetDepth() };

			//Set Start Cap
			Vertex baseStartCap{ combinator({}, -up) };
			generator.SetFlatStartCap(baseStartCap);

			Float3 point{};
			Float3 normal{};

			//right bot back
			point = cube.GetRightBotBack();
			normal = forward;
			generator.AddVertex(combinator(point, normal));

			//left bot back
			point -= rightSize;
			generator.AddVertex(combinator(point, normal));

			normal = -right;
			generator.AddVertex(combinator(point, normal));

			//left bot front
			point -= forwardSize;
			generator.AddVertex(combinator(point, normal));

			normal = -forward;
			generator.AddVertex(combinator(point, normal));

			//right bot front
			point += rightSize;
			generator.AddVertex(combinator(point, normal));

			normal = right;
			generator.AddVertex(combinator(point, normal));

			//right bot back
			point += forwardSize;
			generator.AddVertex(combinator(point, normal));

			//right top back
			point += upSize;
			normal = forward;
			generator.AddVertex(combinator(point, normal));

			//left top back
			point -= rightSize;
			generator.AddVertex(combinator(point, normal));

			normal = -right;
			generator.AddVertex(combinator(point, normal));

			//left top front
			point -= forwardSize;
			generator.AddVertex(combinator(point, normal));

			normal = -forward;
			generator.AddVertex(combinator(point, normal));

			//right top front
			point += rightSize;
			generator.AddVertex(combinator(point, normal));

			normal = right;
			generator.AddVertex(combinator(point, normal));

			//right top back
			point += forwardSize;
			generator.AddVertex(combinator(point, normal));

			Vertex endCapBase{ combinator({}, up) };
			generator.SetFlatEndCap(endCapBase);

			return section;
		}

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

		template<>
		inline Float3 CubeGenerator3Helper::GetRight<CubeAA>(const CubeAA& cube)
		{
			return { 1,0,0 };
		}

		template<>
		inline Float3 CubeGenerator3Helper::GetUp<CubeAA>(const CubeAA& cube)
		{
			return { 0,1,0 };
		}

		template<>
		inline Float3 CubeGenerator3Helper::GetForward<CubeAA>(const CubeAA& cube)
		{
			return { 0,0,1 };
		}
	}
}