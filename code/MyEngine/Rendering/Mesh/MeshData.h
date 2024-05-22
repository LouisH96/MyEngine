#pragma once
#include <Geometry\ModelTopology.h>
#include <DataStructures\List.h>
#include <DataStructures\Adders\ListAdder.h>
#include <DataStructures\Container.h>

namespace MyEngine
{
	//---| Main Classes |---
	template<typename Vertex, ModelTopology Topology>
	class MeshDataWithoutIndices
	{
	public:
		using VertexType = Vertex;
		static constexpr ModelTopology TOPOLOGY = Topology;

		List<Vertex> Vertices;
	};

	template<typename Vertex, ModelTopology Topology>
	class MeshDataWithIndices
	{
	public:
		using VertexType = Vertex;
		static constexpr ModelTopology TOPOLOGY = Topology;

		List<Vertex> Vertices;
		List<int> Indices;
	};

	template<typename Vertex, ModelTopology Topology>
	class MeshData
	{ };

	//---| Specialized Classes |---
	template<typename Vertex>
	class MeshData<Vertex, ModelTopology::TriangleList>
		: public MeshDataWithoutIndices<Vertex, ModelTopology::TriangleList>
	{ };
	template<typename Vertex>
	class MeshData<Vertex, ModelTopology::TriangleStrip>
		: public MeshDataWithoutIndices<Vertex, ModelTopology::TriangleStrip>
	{ };
	template<typename Vertex>
	class MeshData<Vertex, ModelTopology::TriangleListIdx>
		: public MeshDataWithIndices<Vertex, ModelTopology::TriangleListIdx>
	{ };
	template<typename Vertex>
	class MeshData<Vertex, ModelTopology::TriangleStripIdx>
		: public MeshDataWithIndices<Vertex, ModelTopology::TriangleStripIdx>
	{ };

	//---| Extra |---
	class TestAllTopologies
	{
	public:
		template<typename Vertex, typename Generator, typename Combinator, typename Options>
		static void TestAll(Combinator combinator, const Options& options, std::string testName);

		template<typename Vertex, typename Generator, typename Combinator, typename Options, ModelTopology Topology>
		static void Test(Combinator combinator, const Options& options);
	};
	template<typename Vertex, typename Generator, typename Combinator, typename Options>
	inline void TestAllTopologies::TestAll(Combinator combinator, const Options& options, std::string testName)
	{
		std::cout << "Test all topologies for " << testName << std::endl;
		//Test<Vertex, Generator, Combinator, Options, ModelTopology::TriangleList>(combinator, options);
		Test<Vertex, Generator, Combinator, Options, ModelTopology::TriangleListIdx>(combinator, options);
		//Test<Vertex, Generator, Combinator, Options, ModelTopology::TriangleStrip>(combinator, options);
		Test<Vertex, Generator, Combinator, Options, ModelTopology::TriangleStripIdx>(combinator, options);
	}

	template<typename Vertex, typename Generator, typename Combinator, typename Options, ModelTopology Topology>
	inline void TestAllTopologies::Test(Combinator combinator, const Options& options)
	{
		MeshData<Vertex, Topology> data{};
		Generator::Generate(combinator, data, 0, options);

		const unsigned nrVertices{ data.Vertices.GetSize() };
		const unsigned nrIndices{ data.Indices.GetSize() };
		const unsigned memory{ nrVertices * sizeof(Vertex) + nrIndices * sizeof(int) };

		std::cout << "---| "
			<< ModelTopologyToString::ToString<Topology>()
			<< " |---\n"
			<< "\t Vertices: " << nrVertices << "\n"
			<< "\t Indices: " << nrIndices << "\n"
			<< "\t Memory: " << memory << " b\n\n";
	}
}