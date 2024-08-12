#pragma once

#include "MakerData.h"
#include "MakerResult.h"
#include "MakerVertex.h"
#include "Quad\Quad.h"
#include "Quad\QuadMaker.h"
#include "Quad\QuadResult.h"
#include "Strip\StripMaker.h"
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
	MakerVertex<TVertex> FullVertex(const TVertex& vertex) const;
	MakerVertex<TVertex> FullVertex(const Float3& position) const;
	MakerVertex<TVertex> FullVertex(const Float3& position, const Float3& normal) const;
	MakerVertex<TVertex> ConstFullVertex(const Float3& position) const;
	MakerVertex<TVertex> ConstFullVertex(const Float3& position, const Float3& normal) const;
	MakerVertex<TVertex> RefVertex(int index) const;
	MakerVertex<TVertex> ConstRefVertex(int index) const;

	//---| Make Shapes |---
	template<typename TResult = EmptyQuadResult<TVertex, TTopology>>
	TResult Make(Quad<TVertex>& quad);

	template<StripEndStyle TEndStyle, StripEdgeStyle TEdgeStyle>
	MakerResult<TVertex, TTopology> Make(Strip<TVertex, TEndStyle, TEdgeStyle>& strip);

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
inline MakerVertex<TVertex> Maker<TVertex, TTopology>::FullVertex(const TVertex& vertex) const
{
	return MakeVertex<TVertex>::Full(vertex);
}

template<typename TVertex, ModelTopology TTopology>
inline MakerVertex<TVertex> Maker<TVertex, TTopology>::FullVertex(const Float3& position) const
{
	return MakeVertex<TVertex>::Full(position);
}

template<typename TVertex, ModelTopology TTopology>
inline MakerVertex<TVertex> Maker<TVertex, TTopology>::FullVertex(const Float3& position, const Float3& normal) const
{
	return MakeVertex<TVertex>::Full(position, normal);
}

template<typename TVertex, ModelTopology TTopology>
inline MakerVertex<TVertex> Maker<TVertex, TTopology>::ConstFullVertex(const Float3& position) const
{
	return MakeVertex<TVertex>::ConstFull(position);
}

template<typename TVertex, ModelTopology TTopology>
inline MakerVertex<TVertex> Maker<TVertex, TTopology>::ConstFullVertex(const Float3& position, const Float3& normal) const
{
	return MakeVertex<TVertex>::ConstFull(position, normal);
}

template<typename TVertex, ModelTopology TTopology>
inline MakerVertex<TVertex> Maker<TVertex, TTopology>::RefVertex(int index) const
{
	return MakeVertex<TVertex>::Ref(index);
}

template<typename TVertex, ModelTopology TTopology>
inline MakerVertex<TVertex> Maker<TVertex, TTopology>::ConstRefVertex(int index) const
{
	return MakeVertex<TVertex>::ConstRef(index);
}

template<typename TVertex, ModelTopology TTopology>
template<typename TResult>
inline TResult Maker<TVertex, TTopology>::Make(Quad<TVertex>& quad)
{
	QuadMaker<TVertex, TTopology, TResult> maker{ m_Data , quad };
	return maker.Make();
}

template<typename TVertex, ModelTopology TTopology>
template<StripEndStyle TEndStyle, StripEdgeStyle TEdgeStyle>
inline MakerResult<TVertex, TTopology> Maker<TVertex, TTopology>::Make(Strip<TVertex, TEndStyle, TEdgeStyle>& strip)
{
	StripMaker<TVertex, TTopology, TEdgeStyle, TEndStyle> maker{ m_Data, strip };
	return maker.Make();
}

}
}