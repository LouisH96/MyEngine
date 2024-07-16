#pragma once

#include <Geometry\ModelTopology.h>
#include <Rendering\Mesh\MeshData.h>

#include "MakerResult.h"
#include "..\Point.h"
#include "..\TopologyInfo.h"

namespace MyEngine
{
namespace MeshMaker
{

#pragma region Base1

template<typename Vertex, ModelTopology Topology, unsigned ResultSize>
class MakerBase1
{
public:
	MakerBase1(MeshData<Vertex, Topology>& meshData);

protected:
	MakerResult<ResultSize> m_Result;
	MeshData<Vertex, Topology>& m_MeshData;
};

template<typename Vertex, ModelTopology Topology, unsigned ResultSize>
inline MakerBase1<Vertex, Topology, ResultSize>::MakerBase1(MeshData<Vertex, Topology>& meshData)
	: m_MeshData{ meshData }
{
}

#pragma endregion

template<typename Vertex, ModelTopology Topology, unsigned ResultSize, bool HasIndexBuffer>
class MakerBase2;

#pragma region Base2<true> - IndexBuffer

template<typename Vertex, ModelTopology Topology, unsigned ResultSize>
class MakerBase2<Vertex, Topology, ResultSize, true>
	: public MakerBase1<Vertex, Topology, ResultSize>
{
public:
	using DataType = unsigned;
	MakerBase2(MeshData<Vertex, Topology>& meshData);

	template<unsigned ResultIndex>
	void Add(const unsigned& index);

	unsigned Transform(const Point<unsigned>& index);
	unsigned Transform(const Point<Float3>& position);
	unsigned Transform(const Point<Vertex>& vertex);

	template<unsigned InputSize = ResultSize>
	void AddAllToResult(const unsigned* pIndices);

private:
	using BaseClass = MakerBase1<Vertex, Topology, ResultSize>;
};

template<typename Vertex, ModelTopology Topology, unsigned ResultSize>
inline MakerBase2<Vertex, Topology, ResultSize, true>::MakerBase2(MeshData<Vertex, Topology>& meshData)
	: BaseClass{ meshData }
{
}

template<typename Vertex, ModelTopology Topology, unsigned ResultSize>
template<unsigned ResultIndex>
inline void MakerBase2<Vertex, Topology, ResultSize, true>::Add(const unsigned& index)
{
	BaseClass::m_MeshData.Indices.Add(index);
}

template<typename Vertex, ModelTopology Topology, unsigned ResultSize>
template<unsigned InputSize>
inline void MakerBase2<Vertex, Topology, ResultSize, true>::AddAllToResult(const unsigned* pIndices)
{
	for (unsigned i = 0; i < InputSize; i++)
		BaseClass::m_Result.Indices[i] = pIndices[i];
}

template<typename Vertex, ModelTopology Topology, unsigned ResultSize>
inline unsigned MakerBase2<Vertex, Topology, ResultSize, true>::Transform(const Point<unsigned>& index)
{
	return index.Index;
}

template<typename Vertex, ModelTopology Topology, unsigned ResultSize>
inline unsigned MakerBase2<Vertex, Topology, ResultSize, true>::Transform(const Point<Float3>& position)
{
	Vertex vertex{};
	vertex.Pos = position.Pos;
	BaseClass::m_MeshData.Vertices.Add(vertex);
	return BaseClass::m_MeshData.Vertices.GetSize() - 1;
}

template<typename Vertex, ModelTopology Topology, unsigned ResultSize>
inline unsigned MakerBase2<Vertex, Topology, ResultSize, true>::Transform(const Point<Vertex>& point)
{
	BaseClass::m_MeshData.Vertices.Add(point.Vertex);
	return BaseClass::m_MeshData.Vertices.GetSize() - 1;
}

#pragma endregion

#pragma region Base2<false> - No IndexBuffer

template<typename Vertex, ModelTopology Topology, unsigned ResultSize>
class MakerBase2<Vertex, Topology, ResultSize, false>
	: public MakerBase1<Vertex, Topology, ResultSize>
{
public:
	using DataType = Vertex;
	MakerBase2(MeshData<Vertex, Topology>& meshData);

protected:

	template<unsigned ResultIndex>
	void Add(const Vertex& index);

	Vertex Transform(const Point<Float3>& position);
	Vertex Transform(const Point<Vertex>& vertex);

private:
	using BaseClass = MakerBase1<Vertex, Topology, ResultSize>;
};

template<typename Vertex, ModelTopology Topology, unsigned ResultSize>
inline MakerBase2<Vertex, Topology, ResultSize, false>::MakerBase2(MeshData<Vertex, Topology>& meshData)
	: BaseClass{ meshData }
{

}

template<typename Vertex, ModelTopology Topology, unsigned ResultSize>
template<unsigned ResultIndex>
inline void MakerBase2<Vertex, Topology, ResultSize, false>::Add(const Vertex& vertex)
{
	BaseClass::m_Result.Indices[ResultIndex] = BaseClass::m_MeshData.Vertices.GetSize();
	BaseClass::m_MeshData.Vertices.Add(vertex);
}

template<typename Vertex, ModelTopology Topology, unsigned ResultSize>
inline Vertex MakerBase2<Vertex, Topology, ResultSize, false>::Transform(const Point<Float3>& position)
{
	Vertex vertex{};
	vertex.Pos = position.Pos;
	return vertex;
}

template<typename Vertex, ModelTopology Topology, unsigned ResultSize>
inline Vertex MakerBase2<Vertex, Topology, ResultSize, false>::Transform(const Point<Vertex>& vertex)
{
	return vertex.Vertex;
}

#pragma endregion

template<typename Vertex, ModelTopology Topology, unsigned ResultSize, bool HasIndexBuffer>
class MakerBase2
{};

#pragma region Maker

template<typename Vertex, ModelTopology Topology, unsigned TResultSize>
class Maker
	: public MakerBase2<Vertex, Topology, TResultSize, TopologyInfo::HasIndices(Topology)>
{
public:
	static constexpr unsigned ResultSize{ TResultSize };
	using BaseClass = MakerBase2<Vertex, Topology, TResultSize, TopologyInfo::HasIndices(Topology)>;
	using DataType = typename BaseClass::DataType;

	Maker(MeshData<Vertex, Topology>& meshData);

};

template<typename Vertex, ModelTopology Topology, unsigned TResultSize>
inline Maker<Vertex, Topology, TResultSize>::Maker(MeshData<Vertex, Topology>& meshData)
	: BaseClass{ meshData }
{
}

#pragma endregion


}
}
