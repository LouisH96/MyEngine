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

	MakerVertex<TVertex> operator[](unsigned index) const { return Vertices[index]; }

	MakerVertex<TVertex> Vertices[NrVertices];
};

template<typename TVertex>
inline Edge<TVertex>::Edge(MakerVertex<TVertex> first, MakerVertex<TVertex> second)
	: Vertices{ first, second }
{

}

}
}