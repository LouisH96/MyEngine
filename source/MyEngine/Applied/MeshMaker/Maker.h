#pragma once

#include <Geometry\ModelTopology.h>
#include <Rendering\Mesh\MeshData.h>

#include "MakerAdder.h"
#include "MakerVertex.h"
#include "MakerResult.h"
#include "MeshMakerHelper.h"

namespace MyEngine
{
namespace MeshMaker
{
#pragma region Base1

template<typename Vertex, ModelTopology Topology, typename TResult>
class MakerBase1
{
public:
	MakerBase1(MeshData<Vertex, Topology>& meshData, MakerAdder<TResult> adder);

protected:
	MakerAdder<TResult> m_Result;
	MeshData<Vertex, Topology>& m_MeshData;
};

template<typename Vertex, ModelTopology Topology, typename TResult>
inline MakerBase1<Vertex, Topology, TResult>::MakerBase1(
	MeshData<Vertex, Topology>& meshData, MakerAdder<TResult> adder)
	: m_MeshData{ meshData }
	, m_Result{ adder }
{
}

#pragma endregion

template<typename Vertex, ModelTopology Topology, bool HasIndexBuffer, typename TResult>
class MakerBase2;

#pragma region Base2<true> - IndexBuffer

template<typename Vertex, ModelTopology Topology, typename TResult>
class MakerBase2<Vertex, Topology, true, TResult>
	: public MakerBase1<Vertex, Topology, TResult>
{
public:
	using DataType = unsigned;
	MakerBase2(MeshData<Vertex, Topology>& meshData, MakerAdder<TResult> adder);

	unsigned Transform(const MakerVertex& vertex);
	unsigned Transform(const Float3& point);
	void FinishTransformPhase(Array<DataType>& data);
	unsigned Add(const unsigned& index);
	unsigned Add(const unsigned& index, const Float3& normal);
	void RemoveLast();
	void AddAllToResult(PtrRangeConst<unsigned> indices);

private:
	using BaseClass = MakerBase1<Vertex, Topology, TResult>;
};

template<typename Vertex, ModelTopology Topology, typename TResult>
inline MakerBase2<Vertex, Topology, true, TResult>::MakerBase2(
	MeshData<Vertex, Topology>& meshData, MakerAdder<TResult> adder)
	: BaseClass{ meshData, adder }
{
}

template<typename Vertex, ModelTopology Topology, typename TResult>
inline unsigned MakerBase2<Vertex, Topology, true, TResult>::Add(const unsigned& index)
{
	BaseClass::m_MeshData.Indices.Add(index);
	return index;
}

template<typename Vertex, ModelTopology Topology, typename TResult>
inline unsigned MakerBase2<Vertex, Topology, true, TResult>::Add(const unsigned& index, const Float3& normal)
{
	BaseClass::m_MeshData.Indices.Add(index);
	BaseClass::m_MeshData.Vertices[index].Normal = normal;
	return index;
}

template<typename Vertex, ModelTopology Topology, typename TResult>
inline void MakerBase2<Vertex, Topology, true, TResult>::RemoveLast()
{
	BaseClass::m_MeshData.Indices.ReduceSize(1);
}

template<typename Vertex, ModelTopology Topology, typename TResult>
inline void MakerBase2<Vertex, Topology, true, TResult>::AddAllToResult(PtrRangeConst<unsigned> data)
{
	for (unsigned i = 0; i < data.count; i++)
		BaseClass::m_Result.Add(data.pData[i]);
}

template<typename Vertex, ModelTopology Topology, typename TResult>
inline unsigned MakerBase2<Vertex, Topology, true, TResult>::Transform(const MakerVertex& vertex)
{
	if (const MakerPointVertex * pPointVertex{
		dynamic_cast<const MakerPointVertex*>(&vertex) })
	{
		Vertex vertex{};
		vertex.Pos = pPointVertex->Position;
		BaseClass::m_MeshData.Vertices.Add(vertex);
		return BaseClass::m_MeshData.Vertices.GetSize() - 1;
	}
	else if (const MakerFullVertex<Vertex>* pFull =
		dynamic_cast<const MakerFullVertex<Vertex>*>(&vertex))
	{
		BaseClass::m_MeshData.Vertices.Add(pFull->Vertex);
		return BaseClass::m_MeshData.Vertices.GetSize() - 1;
	}
	else if (const MakerRefVertex* pRef =
		dynamic_cast<const MakerRefVertex*>(&vertex))
	{
		return pRef->Index;
	}
	else
	{
		Logger::PrintError("[Maker::Transform] vertex of unknown type");
		return 0;
	}
}
template<typename Vertex, ModelTopology Topology, typename TResult>
inline unsigned MakerBase2<Vertex, Topology, true, TResult>::Transform(const Float3& point)
{
	Vertex vertex{};
	vertex.Pos = point;
	BaseClass::m_MeshData.Vertices.Add(vertex);
	return BaseClass::m_MeshData.Vertices.GetSize() - 1;
}
template<typename Vertex, ModelTopology Topology, typename TResult>
inline void MakerBase2<Vertex, Topology, true, TResult>::FinishTransformPhase(Array<DataType>& data)
{
	AddAllToResult({ data });
}

#pragma endregion

#pragma region Base2<false> - No IndexBuffer

template<typename Vertex, ModelTopology Topology, typename TResult>
class MakerBase2<Vertex, Topology, false, TResult>
	: public MakerBase1<Vertex, Topology, TResult>
{
public:
	using DataType = Vertex;
	MakerBase2(MeshData<Vertex, Topology>& meshData, MakerAdder<TResult> adder);

	Vertex Transform(const MakerVertex& vertex);
	Vertex Transform(const Float3& point);
	void FinishTransformPhase(Array<DataType>& data) {};
	unsigned Add(const Vertex& vertex);
	unsigned Add(const Vertex& vertex, const Float3& normal);
	void RemoveLast();

private:
	using BaseClass = MakerBase1<Vertex, Topology, TResult>;
};

template<typename Vertex, ModelTopology Topology, typename TResult>
inline MakerBase2<Vertex, Topology, false, TResult>::MakerBase2(
	MeshData<Vertex, Topology>& meshData, MakerAdder<TResult> adder)
	: BaseClass{ meshData, adder }
{

}

template<typename Vertex, ModelTopology Topology, typename TResult>
inline unsigned MakerBase2<Vertex, Topology, false, TResult>::Add(const Vertex& vertex)
{
	const unsigned index{ BaseClass::m_MeshData.Vertices.GetSize() };
	BaseClass::m_MeshData.Vertices.Add(vertex);
	BaseClass::m_Result.Add(index);
	return index;
}

template<typename Vertex, ModelTopology Topology, typename TResult>
inline unsigned MakerBase2<Vertex, Topology, false, TResult>::Add(const Vertex& vertex, const Float3& normal)
{
	const unsigned index{ BaseClass::m_MeshData.Vertices.GetSize() };
	BaseClass::m_MeshData.Vertices.Add(vertex);
	BaseClass::m_MeshData.Vertices[index].Normal = normal;
	BaseClass::m_Result.Add(index);
	return index;
}

template<typename Vertex, ModelTopology Topology, typename TResult>
inline void MakerBase2<Vertex, Topology, false, TResult>::RemoveLast()
{
	BaseClass::m_Result.RemoveLast();
	BaseClass::m_MeshData.Vertices.ReduceSize(1);
}

template<typename Vertex, ModelTopology Topology, typename TResult>
inline Vertex MakerBase2<Vertex, Topology, false, TResult>::Transform(const MakerVertex& vertex)
{
	if (const MakerRefVertex* pRef =
		dynamic_cast<const MakerRefVertex*>(&vertex))
	{
		return BaseClass::m_MeshData.Vertices[pRef->Index];
	}
	else if (const MakerPointVertex* pPoint =
		dynamic_cast<const MakerPointVertex*>(&vertex))
	{
		Vertex vertex{};
		vertex.Pos = pPoint->Position;
		return vertex;
	}
	else if (const MakerFullVertex<Vertex>* pFull =
		dynamic_cast<const MakerFullVertex<Vertex>*>(&vertex))
	{
		return pFull->Vertex;
	}
	else
	{
		Logger::PrintError("[Maker<false, TResult>::Transform] unknown vertex");
		return {};
	}
}

template<typename Vertex, ModelTopology Topology, typename TResult>
inline Vertex MakerBase2<Vertex, Topology, false, TResult>::Transform(const Float3& point)
{
	Vertex vertex{};
	vertex.Pos = point;
	return vertex;
}


#pragma endregion

template<typename Vertex, ModelTopology Topology, bool HasIndexBuffer, typename TResult>
class MakerBase2
{};

#pragma region Maker

template<typename Vertex, ModelTopology Topology, typename TResult>
class Maker
	: public MakerBase2<Vertex, Topology, TopologyInfo::HasIndices(Topology), TResult>
{
public:
	using BaseClass = MakerBase2<Vertex, Topology, TopologyInfo::HasIndices(Topology), TResult>;
	using DataType = typename BaseClass::DataType;

	Maker(MeshData<Vertex, Topology>& meshData, MakerAdder<TResult> adder = {});

	void StartShape(); //prepares MeshData-buffers for new shape (only for strip format)

protected:
	void AddSharpQuad(const Array<DataType>& data,
		const Float3& normal,
		unsigned leftBot, unsigned leftTop,
		unsigned rightBot, unsigned rightTop); //careful, need to know the internal when using this
	Float3 GetPosition(SharedPtr<const MakerVertex> vertex);
	Float3 GetPosition(const MakerVertex& vertex);
};

template<typename Vertex, ModelTopology Topology, typename TResult>
inline Maker<Vertex, Topology, TResult>::Maker(
	MeshData<Vertex, Topology>& meshData, MakerAdder<TResult> adder)
	: BaseClass{ meshData, adder }
{
}

template<typename Vertex, ModelTopology Topology, typename TResult>
inline void Maker<Vertex, Topology, TResult>::StartShape()
{
	if constexpr (TopologyInfo::IsListFormat(Topology))
		return;

	if constexpr (TopologyInfo::HasIndices(Topology))
	{
		List<int>& indices{ BaseClass::m_MeshData.Indices };
		if (indices.Empty())
			return;

		indices.Add(-1);

		if (indices.GetSize() % 2 == 1)
			indices.Add(indices.Last());
	}
	else
	{
		List<Vertex>& vertices{ BaseClass::m_MeshData.Vertices };
		if (vertices.Empty())
			return;

		Vertex inf{};
		inf.Pos = Float3{ std::numeric_limits<float>::infinity() };

		vertices.Add(inf);
		if constexpr (TopologyInfo::IsTriangleType(Topology))
			vertices.Add(inf);

		if (vertices.GetSize() % 2 == 1)
			vertices.Add(vertices.Last());
	}
}

#pragma endregion

template<typename Vertex, ModelTopology Topology, typename TResult>
inline void Maker<Vertex, Topology, TResult>::AddSharpQuad(const Array<DataType>& data,
	const Float3& normal,
	unsigned leftBot, unsigned leftTop,
	unsigned rightBot, unsigned rightTop)
{
	if constexpr (TopologyInfo::IsListFormat(Topology))
	{
		if constexpr (TopologyInfo::IsLineType(Topology))
		{
			//LineList
			BaseClass::Add(data[leftBot], normal);
			BaseClass::Add(data[leftTop], normal);

			BaseClass::Add(data[leftTop], normal);
			BaseClass::Add(data[rightTop], normal);

			BaseClass::Add(data[rightTop], normal);
			BaseClass::Add(data[rightBot], normal);

			BaseClass::Add(data[rightBot], normal);
			BaseClass::Add(data[leftBot], normal);
		}
		else
		{
			//TriangleList
			BaseClass::Add(data[leftBot], normal); //left-bot triangle
			BaseClass::Add(data[leftTop], normal);
			BaseClass::Add(data[rightBot], normal);

			BaseClass::Add(data[leftTop], normal); //right-top triangle
			BaseClass::Add(data[rightTop], normal);
			BaseClass::Add(data[rightBot], normal);
		}
	}
	else //FormatType::Strip
	{
		if constexpr (TopologyInfo::IsLineType(Topology))
		{
			//LineStrip
			BaseClass::Add(data[rightBot], normal);
			BaseClass::Add(data[leftBot], normal);
			BaseClass::Add(data[leftTop], normal);
			BaseClass::Add(data[rightTop], normal);
			BaseClass::Add(data[rightBot], normal);
		}
		else
		{
			//TriangleStrip
			BaseClass::Add(data[leftBot], normal);
			BaseClass::Add(data[leftTop], normal);
			BaseClass::Add(data[rightBot], normal);
			BaseClass::Add(data[rightTop], normal);
		}
	}
}

template<typename Vertex, ModelTopology Topology, typename TResult>
inline Float3 Maker<Vertex, Topology, TResult>::GetPosition(SharedPtr<const MakerVertex> vertex)
{
	return GetPosition(vertex.Get());
}

template<typename Vertex, ModelTopology Topology, typename TResult>
inline Float3 Maker<Vertex, Topology, TResult>::GetPosition(const MakerVertex& vertex)
{
	return MeshMakerHelper::GetPosition(vertex, BaseClass::m_MeshData);
}

}
}
