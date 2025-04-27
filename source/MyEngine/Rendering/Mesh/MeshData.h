#pragma once
#include <DataStructures\Adders\ListAdder.h>
#include <DataStructures\Container.h>
#include <DataStructures\List.h>
#include <Geometry\ModelTopology.h>
#include <Geometry\PointUtils.h>
#include <Geometry\Shapes\CubeAA.h>

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

		void StartShape();
		void Clear();
		void Add(const MeshDataWithoutIndices<Vertex, Topology>& other);

		CubeAA GetBounds() const;
	};

	template<typename Vertex, ModelTopology Topology>
	class MeshDataWithIndices
	{
	public:
		using VertexType = Vertex;
		static constexpr ModelTopology TOPOLOGY = Topology;

		List<Vertex> Vertices;
		List<int> Indices;

		void StartShape();
		void Clear();
		void Add(const MeshDataWithIndices<Vertex, Topology>& other);
	};

	template<typename Vertex, ModelTopology Topology>
	void MeshDataWithoutIndices<Vertex, Topology>::StartShape()
	{
		if constexpr (TopologyInfo::IsListFormat(Topology))
			return;

		if (Vertices.Empty())
			return;

		Vertex inf{};
		inf.Pos = Float3{ std::numeric_limits<float>::infinity() };

		Vertices.Add(inf);
		if constexpr (TopologyInfo::IsTriangleType(Topology))
			Vertices.Add(inf);

		if (Vertices.GetSize() % 2 == 1)
			Vertices.Add(Vertices.Last());
	}

	template<typename Vertex, ModelTopology Topology>
	void MeshDataWithoutIndices<Vertex, Topology>::Clear()
	{
		Vertices.Clear();
	}

	template<typename Vertex, ModelTopology Topology>
	void MeshDataWithoutIndices<Vertex, Topology>::Add(const MeshDataWithoutIndices<Vertex, Topology>& other)
	{
		Vertices.Add(other.Vertices);
	}

	template<typename Vertex, ModelTopology Topology>
	inline CubeAA MeshDataWithoutIndices<Vertex, Topology>::GetBounds() const
	{
		//Bounds
		Float3 minBounds{ Float::MAX };
		Float3 maxBounds{ -Float::MAX };
		for (unsigned iPoint{ 0 }; iPoint < Vertices.GetSize(); ++iPoint)
			Geometry::PointUtils::UpdateBounds(
				Vertices[iPoint].Pos, minBounds, maxBounds);

		return CubeAA{minBounds, maxBounds-minBounds};
	}

	template<typename Vertex, ModelTopology Topology>
	void MeshDataWithIndices<Vertex, Topology>::StartShape()
	{
		if constexpr (TopologyInfo::IsListFormat(Topology))
			return;

		if (Indices.Empty())
			return;

		Indices.Add(-1);

		if (Indices.GetSize() % 2 == 1)
			Indices.Add(-1);
	}

	template<typename Vertex, ModelTopology Topology>
	void MeshDataWithIndices<Vertex, Topology>::Clear()
	{
		Vertices.Clear();
		Indices.Clear();
	}

	template<typename Vertex, ModelTopology Topology>
	void MeshDataWithIndices<Vertex, Topology>::Add(const MeshDataWithIndices<Vertex, Topology>& other)
	{
		const unsigned idxOffset{ Vertices.GetSize() };
		Vertices.Add(other.Vertices);

		for (unsigned iIndex{ 0 }; iIndex < other.Indices.GetSize(); ++iIndex)
		{
			const int index{ other.Indices[iIndex] };
			if (index == -1)
				Indices.Add(index);
			else
				Indices.Add(index + idxOffset);
		}
	}

	template<typename Vertex, ModelTopology Topology>
	class MeshData;

	//---| Specialized Classes |---
	template<typename Vertex>
	class MeshData<Vertex, ModelTopology::LineList>
		: public MeshDataWithoutIndices<Vertex, ModelTopology::LineList>
	{ };
	template<typename Vertex>
	class MeshData<Vertex, ModelTopology::LineStrip>
		: public MeshDataWithoutIndices<Vertex, ModelTopology::LineStrip>
	{ };
	template<typename Vertex>
	class MeshData<Vertex, ModelTopology::TriangleList>
		: public MeshDataWithoutIndices<Vertex, ModelTopology::TriangleList>
	{ };
	template<typename Vertex>
	class MeshData<Vertex, ModelTopology::TriangleStrip>
		: public MeshDataWithoutIndices<Vertex, ModelTopology::TriangleStrip>
	{ };

	template<typename Vertex>
	class MeshData<Vertex, ModelTopology::LineListIdx>
		: public MeshDataWithIndices<Vertex, ModelTopology::LineListIdx>
	{ };
	template<typename Vertex>
	class MeshData<Vertex, ModelTopology::LineStripIdx>
		: public MeshDataWithIndices<Vertex, ModelTopology::LineStripIdx>
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