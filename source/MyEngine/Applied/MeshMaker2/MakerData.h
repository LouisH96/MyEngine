#pragma once

#include <Geometry\ModelTopology.h>
#include <Rendering\Mesh\MeshBuffers.h>

namespace MyEngine
{
namespace MeshMaker2
{

#pragma region MakerData - Parts

template<typename TVertex>
class MakerDataVertices
{
public:
	List<TVertex> Vertices;

	const TVertex& operator[](unsigned index) const { return Vertices[index]; }
	TVertex& operator[](unsigned index) { return Vertices[index]; }

	unsigned Add(const TVertex& vertex);
	unsigned GetSize() const { return Vertices.GetSize(); }
};

template<typename TVertex>
inline unsigned MakerDataVertices<TVertex>::Add(const TVertex& vertex)
{
	Vertices.Add(vertex);
	return Vertices.GetSize() - 1;
}

class MakerDataIndices
{
public:
	List<int> Indices;

	int operator[](unsigned index) { return Indices[index]; }

	unsigned Add(int index);
	unsigned GetSize() const { return Indices.GetSize(); }
};

inline unsigned MakerDataIndices::Add(int index)
{
	Indices.Add(index);
	return Indices.GetSize() - 1;
}

#pragma endregion

template<typename TVertex, bool THasIndices>
class MakerDataBase;

template<typename TVertex, ModelTopology TTopology>
using MakerData = MakerDataBase<TVertex, TopologyInfo::HasIndices(TTopology)>;

template<typename TVertex>
class MakerDataBase<TVertex, false>
{
public:
	using ShapeVertex = TVertex;

	MakerDataVertices<TVertex> Vertices;

	int Add(const TVertex& shapeVertex);

	template<ModelTopology TTopology>
	void StartShape();

	template<ModelTopology TTopology>
	void MakeBuffers(MeshBuffers<TVertex, TTopology>& buffers) const;
};

template<typename TVertex>
class MakerDataBase<TVertex, true>
{
public:
	using ShapeVertex = int;

	MakerDataVertices<TVertex> Vertices;
	MakerDataIndices Indices;

	int Add(int shapeVertex);

	template<ModelTopology TTopology>
	void StartShape();

	template<ModelTopology TTopology>
	void MakeBuffers(MeshBuffers<TVertex, TTopology>& buffers) const;
};

template<typename TVertex>
template<ModelTopology TTopology>
inline void MakerDataBase<TVertex, false>::MakeBuffers(MeshBuffers<TVertex, TTopology>& buffers) const
{
	buffers.CreateBuffers(Vertices.Vertices);
}

template<typename TVertex>
inline int MakerDataBase<TVertex, false>::Add(const TVertex& shapeVertex)
{
	Vertices.Add(shapeVertex);
	return Vertices.GetSize() - 1;
}

template<typename TVertex>
template<ModelTopology TTopology>
inline void MakerDataBase<TVertex, false>::StartShape()
{
	if constexpr (TopologyInfo::IsListFormat(TTopology))
		return;

	if (Vertices.GetSize() == 0)
		return;

	TVertex inf{};
	inf.Pos = Float3{ std::numeric_limits<float>::infinity() };

	Vertices.Add(inf);
	if constexpr (TopologyInfo::IsTriangleType(TTopology))
		Vertices.Add(inf);

	if (Vertices.GetSize() % 2 == 1)
		Vertices.Add(inf);
}

template<typename TVertex>
template<ModelTopology TTopology>
inline void MakerDataBase<TVertex, true>::MakeBuffers(MeshBuffers<TVertex, TTopology>& buffers) const
{
	buffers.CreateBuffers(Vertices.Vertices, Indices.Indices);
}

template<typename TVertex>
inline int MakerDataBase<TVertex, true>::Add(int shapeVertex)
{
	Indices.Add(shapeVertex);
	return shapeVertex;
}

template<typename TVertex>
template<ModelTopology TTopology>
inline void MakerDataBase<TVertex, true>::StartShape()
{
	if constexpr (TopologyInfo::IsListFormat(TTopology))
		return;

	if (Indices.GetSize() == 0)
		return;

	Indices.Add(-1);

	if (Indices.GetSize() % 2 == 1)
		Indices.Add(-1);

}

}
}