#pragma once

#include "MakerData.h"
#include "MakerResult.h"
#include "Quad\Quad.h"
#include "Quad\QuadMaker.h"
#include "Quad\QuadResult.h"
#include <Geometry\ModelTopology.h>

namespace MyEngine
{
namespace MeshMaker2
{
template<typename TVertex, ModelTopology TTopology>
class Maker
{
public:
	Maker(MakerData<TVertex, TTopology>& makerData);

	void Begin() { m_Result.Begin(m_Data); }
	void End() { m_Result.End(m_Data); }
	MakerResult<TVertex, TTopology> GetResult()const { return m_Result; }

	//---| Make Vertices |---
	MakerVertex<TVertex> MakeVertex(const Float3& position) const;
	MakerVertex<TVertex> MakeVertex(const Float3& position, const Float3& normal) const;

	//---| Make Shapes |---
	template<typename TResult = EmptyQuadResult<TVertex, TTopology>>
	TResult Make(Quad<TVertex>& quad);

	//---| Get |---
	MakerData<TVertex, TTopology>& GetData() { return m_Data; }
	const MakerData<TVertex, TTopology>& GetData() const { return m_Data; }

private:
	MakerResult<TVertex, TTopology> m_Result{};
	MakerData<TVertex, TTopology>& m_Data{};
};

template<typename TVertex, ModelTopology TTopology>
inline Maker<TVertex, TTopology>::Maker(MakerData<TVertex, TTopology>& makerData)
	: m_Data{ makerData }
{
}

template<typename TVertex, ModelTopology TTopology>
inline MakerVertex<TVertex> Maker<TVertex, TTopology>::MakeVertex(const Float3& position) const
{
	return FullMakerVertex<TVertex>{position};
}

template<typename TVertex, ModelTopology TTopology>
inline MakerVertex<TVertex> Maker<TVertex, TTopology>::MakeVertex(const Float3& position, const Float3& normal) const
{
	return FullMakerVertex<TVertex>{position, normal};
}

template<typename TVertex, ModelTopology TTopology>
template<typename TResult>
inline TResult Maker<TVertex, TTopology>::Make(Quad<TVertex>& quad)
{
	QuadMaker<TVertex, TTopology, TResult> maker{ m_Data , quad };
	return maker.Make();
}

}
}