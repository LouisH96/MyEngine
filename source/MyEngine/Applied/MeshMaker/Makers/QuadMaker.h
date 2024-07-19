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
	: public Maker<Vertex, Topology>
{
public:
	using BaseClass = Maker<Vertex, Topology>;
	using typename BaseClass::DataType;

	QuadMaker(MeshData<Vertex, Topology>& meshData);

	MakerResult Make(const Quad& quad);
};

template<typename Vertex, ModelTopology Topology>
inline QuadMaker<Vertex, Topology>::QuadMaker(MeshData<Vertex, Topology>& meshData)
	: BaseClass{ meshData }
{
}
template<typename Vertex, ModelTopology Topology>
inline MakerResult QuadMaker<Vertex, Topology>::Make(const Quad& quad)
{
	constexpr TopologyInfo::BaseType baseType{ TopologyInfo::GetBaseType(Topology) };

	const DataType data[]{
		BaseClass::Transform(&quad.Vertices[0].Get()),
		BaseClass::Transform(&quad.Vertices[1].Get()),
		BaseClass::Transform(&quad.Vertices[2].Get()),
		BaseClass::Transform(&quad.Vertices[3].Get()),
	};

	if constexpr (TopologyInfo::HasIndices(Topology))
		BaseClass::AddAllToResult(PtrRangeConst<unsigned>{data, sizeof(data) / sizeof(const DataType)});

	if constexpr (baseType == TopologyInfo::BaseType::LineList)
	{
		BaseClass::Add(data[0]);
		BaseClass::Add(data[1]);

		BaseClass::Add(data[1]);
		BaseClass::Add(data[2]);

		BaseClass::Add(data[2]);
		BaseClass::Add(data[3]);

		BaseClass::Add(data[3]);
		BaseClass::Add(data[0]);
	}
	else if constexpr (baseType == TopologyInfo::BaseType::LineStrip)
	{
		BaseClass::Add(data[0]);
		BaseClass::Add(data[1]);
		BaseClass::Add(data[2]);

		BaseClass::Add(data[3]);
		BaseClass::Add(data[0]);
	}
	else if constexpr (baseType == TopologyInfo::BaseType::TriangleList)
	{
		BaseClass::Add(data[0]);
		BaseClass::Add(data[1]);
		BaseClass::Add(data[2]);

		BaseClass::Add(data[2]);
		BaseClass::Add(data[3]);
		BaseClass::Add(data[0]);
	}
	else //TriangleStrip
	{
		BaseClass::Add(data[1]);
		BaseClass::Add(data[2]);
		BaseClass::Add(data[0]);

		BaseClass::Add(data[3]);
	}

	return BaseClass::m_Result;
}
}
}