#pragma once

#include <Rendering\Mesh\MeshData.h>
#include "Shapes\Line.h"
#include "MakerVertex.h"

namespace MyEngine
{
namespace MeshMaker
{
class MeshMakerHelper
{
public:
	template<typename TVertex, ModelTopology Topology>
	static Float3 GetPosition(const MakerVertex& makerVertex, const MeshData<TVertex, Topology>& meshData);

	template<typename TVertex, ModelTopology TTopology>
	static Float3 GetNormal(const Line& leftEdge, const Line& rightEdge, const MeshData<TVertex, TTopology>& meshData);

	template<typename TVertex, ModelTopology TTopology>
	static SharedPtr<const MakerVertex> GetMoved(
		const SharedPtr<const MakerVertex> makerVertex,
		const Float3& displacement,
		const MeshData<TVertex, TTopology>& meshData);
};
template<typename TVertex, ModelTopology Topology>
inline Float3 MeshMakerHelper::GetPosition(const MakerVertex& makerVertex, const MeshData<TVertex, Topology>& meshData)
{
	if (const MakerRefVertex* pRef =
		dynamic_cast<const MakerRefVertex*>(&makerVertex))
	{
		return meshData.Vertices[pRef->Index].Pos;
	}
	else if (const MakerFullVertex<TVertex>* pVertex =
		dynamic_cast<const MakerFullVertex<TVertex>*>(&makerVertex))
	{
		return pVertex->Vertex.Pos;
	}
	else if (const MakerPointVertex* pPoint =
		dynamic_cast<const MakerPointVertex*>(&makerVertex))
	{
		return pPoint->Position;
	}
	else
	{
		Logger::PrintError("[MeshMakerHelper::GetPosition] unknown MakerVertex-type");
		return {};
	}
}
template<typename TVertex, ModelTopology TTopology>
inline Float3 MeshMakerHelper::GetNormal(const Line& leftEdge, const Line& rightEdge,
	const MeshData<TVertex, TTopology>& meshData)
{
	const Float3 leftBotPos{ MeshMakerHelper::GetPosition(leftEdge[0].Get(), meshData) };
	const Float3 leftTopPos{ MeshMakerHelper::GetPosition(leftEdge[1].Get(), meshData) };
	const Float3 rightBotPos{ MeshMakerHelper::GetPosition(rightEdge[0].Get(), meshData) };

	const Float3 toLeftTop{ leftTopPos - leftBotPos };
	const Float3 toRightBot{ rightBotPos - leftBotPos };

	return toLeftTop.Cross(toRightBot).Normalized();
}
template<typename TVertex, ModelTopology TTopology>
inline SharedPtr<const MakerVertex> MeshMakerHelper::GetMoved(
	const SharedPtr<const MakerVertex> makerVertex,
	const Float3& displacement,
	const MeshData<TVertex, TTopology>& meshData)
{
	if (const MakerRefVertex* pRef =
		dynamic_cast<const MakerRefVertex*>(&makerVertex.Get()))
	{
		const TVertex& vertex{ meshData.Vertices[pRef->Index] };

		TVertex newVertex{ vertex };
		newVertex.Pos += displacement;

		return SharedPtr<const MakerFullVertex<TVertex>>{
			MakerFullVertex<TVertex>{newVertex}};
	}
	else if (const MakerFullVertex<TVertex>* pVertex =
		dynamic_cast<const MakerFullVertex<TVertex>*>(&makerVertex.Get()))
	{
		TVertex newVertex{ pVertex->Vertex };
		newVertex.Pos += displacement;

		return SharedPtr<const MakerFullVertex<TVertex>>{
			MakerFullVertex<TVertex>{newVertex}};
	}
	else if (const MakerPointVertex* pPoint =
		dynamic_cast<const MakerPointVertex*>(&makerVertex.Get()))
	{
		return SharedPtr<const MakerPointVertex>{MakerPointVertex{
			pPoint->Position + displacement }	};
	}
	else
	{
		Logger::PrintError("[MeshMakerHelper::GetMoved] unknown MakerVertex-type");
		return {};
	}
}
}
}

