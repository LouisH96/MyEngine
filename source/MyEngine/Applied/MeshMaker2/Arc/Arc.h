#pragma once

#include "..\MakerVertex.h"
#include "..\MakerData.h"

namespace MyEngine
{
namespace MeshMaker2
{

enum class ArcEdgeStyle {
	Shared, Split
};

template<typename TVertex, ArcEdgeStyle TEdgeStyle>
class Arc;

template<typename TVertex>
using SharedArc = Arc<TVertex, ArcEdgeStyle::Shared>;

template<typename TVertex>
using SplitArc = Arc<TVertex, ArcEdgeStyle::Split>;

template<typename TVertex>
class Arc<TVertex, ArcEdgeStyle::Shared>
{
public:
	using MakerVertex = MakerVertex<TVertex>;

	Arc();

	/* stores the center & left only on first call*/
	void AddWing(MakerVertex center, MakerVertex left, MakerVertex right);

	template<bool THasIndices>
	void CalculateNormals(MakerDataBase<TVertex, THasIndices>& data);

	MakerVertex GetWingCenter(unsigned iWing) const { return m_Vertices[0]; }
	MakerVertex GetWingLeft(unsigned iWing) const { return m_Vertices[iWing + 1]; }
	MakerVertex GetWingRight(unsigned iWing) const { return m_Vertices[iWing + 2]; }
	unsigned GetNrWings() const { return (m_Vertices.GetSize() - 2); }

private:
	List<MakerVertex> m_Vertices; //first one is center
};

template<typename TVertex>
inline Arc<TVertex, ArcEdgeStyle::Shared>::Arc()
{
	m_Vertices.Add({});
}

template<typename TVertex>
inline void Arc<TVertex, ArcEdgeStyle::Shared>::AddWing(
	MakerVertex center, MakerVertex left, MakerVertex right)
{
	if (m_Vertices.GetSize() == 1)
	{
		m_Vertices.First() = center;
		m_Vertices.Add(left);
	}
	m_Vertices.Add(right);
}

template<typename TVertex>
template<bool THasIndices>
inline void Arc<TVertex, ArcEdgeStyle::Shared>::CalculateNormals(MakerDataBase<TVertex, THasIndices>& data)
{
	//todo: Arc<Shared>::CalculateNormals (requires Slerp)

	//doesn't use Slerp so it's not accurate
	const Float3& center{ m_Vertices[0].GetPosition(data) };

	Array<Float3> wingNormals{ GetNrWings() };
	for (unsigned iWing{ 0 }; iWing < GetNrWings(); ++iWing)
	{
		const Float3& left{ GetWingLeft(iWing).GetPosition(data) };
		const Float3& right{ GetWingRight(iWing).GetPosition(data) };

		const Float3 toLeft{ left - center };
		const Float3 toRight{ right - center };
		const Float3 normal{ toLeft.Cross(toRight).Normalized() };
		wingNormals[iWing] = normal;
	}

	//
	GetWingCenter(0).TrySetNormal(Float3{ 0,0,0 }, data);
	GetWingLeft(0).TrySetNormal(wingNormals.First(), data);
	GetWingRight(GetNrWings() - 1).TrySetNormal(wingNormals.Last(), data);
	for (unsigned iWing{ 1 }; iWing < GetNrWings(); ++iWing)
	{
		const Float3& leftNormal{ wingNormals[iWing - 1] };
		const Float3& rightNormal{ wingNormals[iWing] };

		const Float3 lerp{ (leftNormal + rightNormal) / 2 };
		GetWingLeft(iWing).TrySetNormal(lerp, data);
	}
}

template<typename TVertex>
class Arc<TVertex, ArcEdgeStyle::Split>
{
public:
	using MakerVertex = MakerVertex<TVertex>;

	Arc() = default;

	void AddWing(MakerVertex center, MakerVertex left, MakerVertex right);

	template<bool THasIndices>
	void CalculateNormals(MakerDataBase<TVertex, THasIndices>& data);

	MakerVertex GetWingCenter(unsigned iWing) const { return m_Vertices[iWing * 3]; }
	MakerVertex GetWingLeft(unsigned iWing) const { return m_Vertices[iWing * 3 + 1]; }
	MakerVertex GetWingRight(unsigned iWing) const { return m_Vertices[iWing * 3 + 2]; }
	unsigned GetNrWings() const { return m_Vertices.GetSize() / 3; }

private:
	List<MakerVertex> m_Vertices;
};

template<typename TVertex>
inline void Arc<TVertex, ArcEdgeStyle::Split>::AddWing(MakerVertex center, MakerVertex left, MakerVertex right)
{
	m_Vertices.Add(center, left, right);
}

template<typename TVertex>
template<bool THasIndices>
inline void Arc<TVertex, ArcEdgeStyle::Split>::CalculateNormals(MakerDataBase<TVertex, THasIndices>& data)
{
	for (unsigned iWing{ 0 }; iWing < GetNrWings(); ++iWing)
	{
		const Float3& center{ GetWingCenter(iWing).GetPosition(data) };
		const Float3& left{ GetWingLeft(iWing).GetPosition(data) };
		const Float3& right{ GetWingRight(iWing).GetPosition(data) };

		const Float3 toLeft{ left - center };
		const Float3 toRight{ right - center };
		const Float3 normal{ toLeft.Cross(right).Normalized() };

		GetWingCenter(iWing).TrySetNormal(normal, data);
		GetWingLeft(iWing).TrySetNormal(normal, data);
		GetWingRight(iWing).TrySetNormal(normal, data);
	}
}

}
}