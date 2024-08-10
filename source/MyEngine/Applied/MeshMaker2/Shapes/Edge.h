#pragma once

#include "..\MakerVertex.h"

namespace MyEngine
{
namespace MeshMaker2
{
template<typename TVertex>
class Edge
{
public:
	static constexpr unsigned NrVertices{ 2 };

	Edge() = default;
	Edge(MakerVertex<TVertex> first, MakerVertex<TVertex> second);

	template<bool THasIndices>
	Edge(const Float3& first, const Float3& second, 
		const Float3& normal, MakerDataBase<TVertex, THasIndices>& data);

	MakerVertex<TVertex> operator[](unsigned index) const { return Vertices[index]; }
	MakerVertex<TVertex> Vertices[NrVertices];
	
};

template<typename TVertex>
inline Edge<TVertex>::Edge(MakerVertex<TVertex> first, MakerVertex<TVertex> second)
	: Vertices{ first, second }
{

}

template<typename TVertex>
template<bool THasIndices>
inline Edge<TVertex>::Edge(const Float3& first, const Float3& second, const Float3& normal, MakerDataBase<TVertex, THasIndices>& data)
	: Vertices{first, second}
{
	Vertices[0].TrySetNormal(normal, data);
	Vertices[1].TrySetNormal(normal, data);
}

}
}