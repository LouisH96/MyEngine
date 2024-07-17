#pragma once

#include "..\Quad.h"
#include "Maker.h"
#include "MakerResult.h"

namespace MyEngine
{
namespace MeshMaker
{
class QuadMakerHelper
{
public:
	static constexpr unsigned GetNrVertices(ModelTopology Topology);
};

constexpr unsigned QuadMakerHelper::GetNrVertices(ModelTopology Topology)
{
	if (TopologyInfo::HasIndices(Topology))
		return 4;

	switch (TopologyInfo::GetBaseType(Topology))
	{
	case TopologyInfo::BaseType::LineList: return 2 * 4;
	case TopologyInfo::BaseType::LineStrip: return 5;
	case TopologyInfo::BaseType::TriangleList: return 2 * 3;
	case TopologyInfo::BaseType::TriangleStrip: return 4;
	}
}

template<typename Vertex, ModelTopology Topology>
class QuadMaker
	: public Maker<Vertex, Topology, QuadMakerHelper::GetNrVertices(Topology)>
{
public:
	using BaseClass = Maker<Vertex, Topology, QuadMakerHelper::GetNrVertices(Topology)>;
	using BaseClass::ResultSize;
	using typename BaseClass::DataType;
	using Result = typename MakerResult<ResultSize>;

	QuadMaker(MeshData<Vertex, Topology>& meshData);

	Result Make(const Quad& quad);
};

template<typename Vertex, ModelTopology Topology>
inline QuadMaker<Vertex, Topology>::QuadMaker(MeshData<Vertex, Topology>& meshData)
	: BaseClass{ meshData }
{
}
template<typename Vertex, ModelTopology Topology>
inline typename QuadMaker<Vertex, Topology>::Result QuadMaker<Vertex, Topology>::Make(const Quad& quad)
{
	constexpr TopologyInfo::BaseType baseType{ TopologyInfo::GetBaseType(Topology) };

	const DataType data[]{
		BaseClass::Transform(quad.pVertices[0]),
		BaseClass::Transform(quad.pVertices[1]),
		BaseClass::Transform(quad.pVertices[2]),
		BaseClass::Transform(quad.pVertices[3]),
	};

	if constexpr (TopologyInfo::HasIndices(Topology))
		BaseClass::AddAllToResult(data);

	if constexpr (baseType == TopologyInfo::BaseType::LineList)
	{
		BaseClass::Add<0>(data[0]);
		BaseClass::Add<1>(data[1]);

		BaseClass::Add<2>(data[1]);
		BaseClass::Add<3>(data[2]);

		BaseClass::Add<4>(data[2]);
		BaseClass::Add<5>(data[3]);

		BaseClass::Add<6>(data[3]);
		BaseClass::Add<7>(data[0]);
	}
	else if constexpr (baseType == TopologyInfo::BaseType::LineStrip)
	{
		BaseClass::Add<0>(data[0]);
		BaseClass::Add<1>(data[1]);
		BaseClass::Add<2>(data[2]);

		BaseClass::Add<3>(data[3]);
		BaseClass::Add<4>(data[0]);
	}
	else if constexpr (baseType == TopologyInfo::BaseType::TriangleList)
	{
		BaseClass::Add<0>(data[0]);
		BaseClass::Add<1>(data[1]);
		BaseClass::Add<2>(data[2]);

		BaseClass::Add<3>(data[2]);
		BaseClass::Add<4>(data[3]);
		BaseClass::Add<5>(data[0]);
	}
	else //TriangleStrip
	{
		BaseClass::Add<1>(data[1]);
		BaseClass::Add<2>(data[2]);
		BaseClass::Add<0>(data[0]);

		BaseClass::Add<3>(data[3]);
	}

	return BaseClass::m_Result;
}
}
}