#pragma once

#include "Quad.h"
#include <DataStructures\StackArray.h>
#include <Geometry\ModelTopology.h>

namespace MyEngine
{
namespace MeshMaker2
{
template<typename TVertex, ModelTopology TTopology, typename TResult>
class QuadMaker
{
public:
	QuadMaker(
		MakerData<TVertex, TTopology>& makerData,
		Quad<TVertex>& quad);

	TResult Make();

private:
	static constexpr unsigned NrShapeVertices{ 4 };
	using ShapeVertex = typename MakerData<TVertex, TTopology>::ShapeVertex;
	using ShapeVertices = StackArray<ShapeVertex, NrShapeVertices>;

	struct AddHelper {
		AddHelper(QuadMaker<TVertex, TTopology, TResult>& quadMaker, const ShapeVertices& vertices)
			: Maker{ quadMaker }, Vertices{ vertices } {};

		QuadMaker<TVertex, TTopology, TResult>& Maker;
		const ShapeVertices& Vertices;

		void AddLeftBot() { Maker.m_Result.AddLeftBot(Maker.m_Data.Add(Vertices[0])); }
		void AddLeftTop() { Maker.m_Result.AddLeftTop(Maker.m_Data.Add(Vertices[1])); }
		void AddRightBot() { Maker.m_Result.AddRightBot(Maker.m_Data.Add(Vertices[2])); }
		void AddRightTop() { Maker.m_Result.AddRightTop(Maker.m_Data.Add(Vertices[3])); }
	};
	friend struct AddHelper;

	MakerData<TVertex, TTopology>& m_Data;
	Quad<TVertex>& m_Quad;
	TResult m_Result;

	ShapeVertices CreateShapeVertices();

	void Combine(AddHelper helper, TopologyBaseType::LineList);
	void Combine(AddHelper helper, TopologyBaseType::LineStrip);
	void Combine(AddHelper helper, TopologyBaseType::TriangleList);
	void Combine(AddHelper helper, TopologyBaseType::TriangleStrip);
};

template<typename TVertex, ModelTopology TTopology, typename TResult>
inline QuadMaker<TVertex, TTopology, TResult>::QuadMaker(
	MakerData<TVertex, TTopology>& makerData,
	Quad<TVertex>& quad)
	: m_Data{ makerData }
	, m_Quad{ quad }
{

}

template<typename TVertex, ModelTopology TTopology, typename TResult>
inline TResult QuadMaker<TVertex, TTopology, TResult>::Make()
{
	//create result
	m_Result.Begin(m_Data);

	//set normals
	m_Quad.GetLeftBot().SetNormal(m_Quad.GetNormal(), m_Data);
	m_Quad.GetLeftTop().SetNormal(m_Quad.GetNormal(), m_Data);
	m_Quad.GetRightBot().SetNormal(m_Quad.GetNormal(), m_Data);
	m_Quad.GetRightTop().SetNormal(m_Quad.GetNormal(), m_Data);

	//create shape vertices
	const StackArray<ShapeVertex, NrShapeVertices> shapeVertices{
		CreateShapeVertices()
	};

	//combine shape vertices
	Combine(AddHelper{ *this , shapeVertices }, TopologyBaseType::FromModelTopology<TTopology> {});

	//return result
	m_Result.End(m_Data);
	return m_Result;
}

template<typename TVertex, ModelTopology TTopology, typename TResult>
inline typename QuadMaker<TVertex, TTopology, TResult>::ShapeVertices
QuadMaker<TVertex, TTopology, TResult>::CreateShapeVertices()
{
	StackArray<ShapeVertex, NrShapeVertices> vertices{};
	vertices[0] = m_Quad.GetLeftBot().MakeShapeVertex(m_Data);
	vertices[1] = m_Quad.GetLeftTop().MakeShapeVertex(m_Data);
	vertices[2] = m_Quad.GetRightBot().MakeShapeVertex(m_Data);
	vertices[3] = m_Quad.GetRightTop().MakeShapeVertex(m_Data);
	return vertices;
}

template<typename TVertex, ModelTopology TTopology, typename TResult>
inline void QuadMaker<TVertex, TTopology, TResult>::Combine(AddHelper helper, TopologyBaseType::LineList)
{
	//Vertical
	helper.AddLeftBot();
	helper.AddLeftTop();
	helper.AddRightBot();
	helper.AddRightTop();

	//Horizontal
	helper.AddLeftBot();
	helper.AddRightBot();
	helper.AddLeftTop();
	helper.AddRightTop();
}

template<typename TVertex, ModelTopology TTopology, typename TResult>
inline void QuadMaker<TVertex, TTopology, TResult>::Combine(AddHelper helper, TopologyBaseType::LineStrip)
{
	helper.AddLeftBot();
	helper.AddLeftTop();
	helper.AddRightTop();
	helper.AddRightBot();
	helper.AddLeftBot();
}

template<typename TVertex, ModelTopology TTopology, typename TResult>
inline void QuadMaker<TVertex, TTopology, TResult>::Combine(AddHelper helper, TopologyBaseType::TriangleList)
{
	helper.AddLeftBot();
	helper.AddLeftTop();
	helper.AddRightTop();

	helper.AddLeftBot();
	helper.AddRightTop();
	helper.AddRightBot();
}

template<typename TVertex, ModelTopology TTopology, typename TResult>
inline void QuadMaker<TVertex, TTopology, TResult>::Combine(AddHelper helper, TopologyBaseType::TriangleStrip)
{
	helper.AddLeftTop();
	helper.AddRightTop();
	helper.AddLeftBot();
	helper.AddRightBot();
}

}
}