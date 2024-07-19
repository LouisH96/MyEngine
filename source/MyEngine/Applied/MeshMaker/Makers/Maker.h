#pragma once

#include <Geometry\ModelTopology.h>
#include <Rendering\Mesh\MeshData.h>

#include "MakerResult.h"
#include "..\MakerVertex.h"

namespace MyEngine
{
namespace MeshMaker
{

#pragma region Base1

template<typename Vertex, ModelTopology Topology>
class MakerBase1
{
public:
	MakerBase1(MeshData<Vertex, Topology>& meshData);

protected:
	MakerResult m_Result;
	MeshData<Vertex, Topology>& m_MeshData;
};

template<typename Vertex, ModelTopology Topology>
inline MakerBase1<Vertex, Topology>::MakerBase1(MeshData<Vertex, Topology>& meshData)
	: m_MeshData{ meshData }
{
}

#pragma endregion

template<typename Vertex, ModelTopology Topology, bool HasIndexBuffer>
class MakerBase2;

#pragma region Base2<true> - IndexBuffer

template<typename Vertex, ModelTopology Topology>
class MakerBase2<Vertex, Topology, true>
	: public MakerBase1<Vertex, Topology>
{
public:
	using DataType = unsigned;
	MakerBase2(MeshData<Vertex, Topology>& meshData);

	void Add(const unsigned& index);

	unsigned Transform(const MakerVertex* pVertex);

	void AddAllToResult(PtrRangeConst<unsigned> indices);

private:
	using BaseClass = MakerBase1<Vertex, Topology>;
};

template<typename Vertex, ModelTopology Topology>
inline MakerBase2<Vertex, Topology, true>::MakerBase2(MeshData<Vertex, Topology>& meshData)
	: BaseClass{ meshData }
{
}

template<typename Vertex, ModelTopology Topology>
inline void MakerBase2<Vertex, Topology, true>::Add(const unsigned& index)
{
	BaseClass::m_MeshData.Indices.Add(index);
}

template<typename Vertex, ModelTopology Topology>
inline void MakerBase2<Vertex, Topology, true>::AddAllToResult(PtrRangeConst<unsigned> data)
{
	for (unsigned i = 0; i < data.count; i++)
		BaseClass::m_Result.Add(data.pData[i]);
}

template<typename Vertex, ModelTopology Topology>
inline unsigned MakerBase2<Vertex, Topology, true>::Transform(const MakerVertex* pVertex)
{
	if (const MakerPointVertex * pPointVertex{
		dynamic_cast<const MakerPointVertex*>(pVertex) })
	{
		Vertex vertex{};
		vertex.Pos = pPointVertex->Position;
		BaseClass::m_MeshData.Vertices.Add(vertex);
		return BaseClass::m_MeshData.Vertices.GetSize() - 1;
	}
	else if (const MakerFullVertex<Vertex>* pFull =
		dynamic_cast<const MakerFullVertex<Vertex>*>(pVertex))
	{
		BaseClass::m_MeshData.Vertices.Add(pFull->Vertex);
		return BaseClass::m_MeshData.Vertices.GetSize() - 1;
	}
	else if (const MakerRefVertex* pRef =
		dynamic_cast<const MakerRefVertex*>(pVertex))
	{
		return pRef->Index;
	}
	else
	{
		Logger::PrintError("[Maker::Transform] vertex of unknown type");
		return 0;
	}
}

#pragma endregion

#pragma region Base2<false> - No IndexBuffer

template<typename Vertex, ModelTopology Topology>
class MakerBase2<Vertex, Topology, false>
	: public MakerBase1<Vertex, Topology>
{
public:
	using DataType = Vertex;
	MakerBase2(MeshData<Vertex, Topology>& meshData);

protected:

	void Add(const Vertex& index);

	Vertex Transform(const MakerVertex* pVertex);

private:
	using BaseClass = MakerBase1<Vertex, Topology>;
};

template<typename Vertex, ModelTopology Topology>
inline MakerBase2<Vertex, Topology, false>::MakerBase2(MeshData<Vertex, Topology>& meshData)
	: BaseClass{ meshData }
{

}

template<typename Vertex, ModelTopology Topology>
inline void MakerBase2<Vertex, Topology, false>::Add(const Vertex& vertex)
{
	BaseClass::m_Result.Indices.Add(BaseClass::m_MeshData.Vertices.GetSize());
	BaseClass::m_MeshData.Vertices.Add(vertex);
}

template<typename Vertex, ModelTopology Topology>
inline Vertex MakerBase2<Vertex, Topology, false>::Transform(const MakerVertex* pVertex)
{
	if (const MakerRefVertex* pRef =
		dynamic_cast<const MakerRefVertex*>(pVertex))
	{
		return BaseClass::m_MeshData.Vertices[pRef->Index];
	}
	else if (const MakerPointVertex* pPoint =
		dynamic_cast<const MakerPointVertex*>(pVertex))
	{
		Vertex vertex{};
		vertex.Pos = pPoint->Position;
		return vertex;
	}
	else if (const MakerFullVertex<Vertex>* pFull =
		dynamic_cast<const MakerFullVertex<Vertex>*>(pVertex))
	{
		return pFull->Vertex;
	}
	else
	{
		Logger::PrintError("[Maker<false>::Transform] unknown vertex");
		return {};
	}
}

#pragma endregion

template<typename Vertex, ModelTopology Topology, bool HasIndexBuffer>
class MakerBase2
{};

#pragma region Maker

template<typename Vertex, ModelTopology Topology>
class Maker
	: public MakerBase2<Vertex, Topology, TopologyInfo::HasIndices(Topology)>
{
public:
	using BaseClass = MakerBase2<Vertex, Topology, TopologyInfo::HasIndices(Topology)>;
	using DataType = typename BaseClass::DataType;

	Maker(MeshData<Vertex, Topology>& meshData);

};

template<typename Vertex, ModelTopology Topology>
inline Maker<Vertex, Topology>::Maker(MeshData<Vertex, Topology>& meshData)
	: BaseClass{ meshData }
{
}

#pragma endregion

}
}
