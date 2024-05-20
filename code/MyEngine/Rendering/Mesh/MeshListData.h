#pragma once
#include <Geometry\ModelTopology.h>
#include <DataStructures\List.h>
#include <DataStructures\Adders\ListAdder.h>

namespace MyEngine
{
	//---| Main Classes |---
	template<typename Vertex>
	class MeshListDataWithoutIndices
	{
	public:
		List<int> Indices;
	};

	template<typename Vertex>
	class MeshListDataWithIndices
	{
	public:
		ListAdder<Vertex> GetVertexAdder();
		ListAdder<int> GetIndexAdder();

		List<Vertex> Vertices;
		List<int> Indices;
	};

	template<typename Vertex, ModelTopology Topology>
	class MeshListData
	{ };

	//---| Specialized Classes |---
	template<typename Vertex>
	class MeshListData<Vertex, ModelTopology::TriangleListIdx>
		: public MeshListDataWithIndices<Vertex>
	{ };

	template<typename Vertex>
	inline ListAdder<Vertex> MeshListDataWithIndices<Vertex>::GetVertexAdder()
	{
		return ListAdder<Vertex>{Vertices};
	}

	template<typename Vertex>
	inline ListAdder<int> MeshListDataWithIndices<Vertex>::GetIndexAdder()
	{
		return ListAdder<int>(Indices);
	}

}