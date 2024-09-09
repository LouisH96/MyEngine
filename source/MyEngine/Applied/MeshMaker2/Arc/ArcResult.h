#pragma once
#include <Geometry\ModelTopology.h>

namespace MyEngine
{
namespace MeshMaker2
{

template<typename TVertex, ModelTopology TTopology>
class EmptyArcResult
	: public MakerResult<TVertex, TTopology>
{
public:
	void AddCenter(int) {};
	void AddCorner(int, int) {};
};

template<typename TVertex, ModelTopology TTopology>
class ArcResult
	: public MakerResult<TVertex, TTopology>
{
public:
	ArcResult() : m_Indices{} { m_Indices.Add(Int::MAX); };

	void AddCenter(int index) { m_Indices[0] = index; }
	void AddCorner(int iCorner, int index) {
		if (m_Indices.GetSizeS() <= iCorner + 1)
			m_Indices.Add(index);
	}

	int GetCenter() const { return m_Indices.First(); }
	int GetCorner(int iCorner) const { return m_Indices[iCorner + 1]; }

private:
	List<int> m_Indices{};
};

}
}