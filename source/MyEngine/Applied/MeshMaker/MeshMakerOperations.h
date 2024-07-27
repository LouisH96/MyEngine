#pragma once

#include "Shapes\Strip.h"
#include "Shapes\StripLoop.h"

namespace MyEngine
{
namespace MeshMaker
{
class MeshMakerOperations
{
public:
	template<typename TVertex, ModelTopology TTopology>
	static StripLoop StripLoop(const Strip& strip, const MeshData<TVertex, TTopology>& meshData);
};

template<typename TVertex, ModelTopology TTopology>
inline StripLoop MeshMakerOperations::StripLoop(const Strip& strip, const MeshData<TVertex, TTopology>& meshData)
{
    MeshMaker::StripLoop loop{};

    loop.SetEdges(strip.GetEdges());
    loop.SetNormals(strip.GetNormals());
    loop.CalculateMissingNormals_Sharp(meshData);

    return loop;
}
}
}