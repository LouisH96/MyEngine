#pragma once

#include "..\MakerVertex.h"
#include "..\Shapes\Edge.h"
#include "..\MakerData.h"

namespace MyEngine
{
namespace MeshMaker2
{
template<typename TVertex>
class Quad
{
private:
	static constexpr unsigned IDX_LEFT_BOT{ 0 };
	static constexpr unsigned IDX_LEFT_TOP{ 1 };
	static constexpr unsigned IDX_RIGHT_TOP{ 2 };
	static constexpr unsigned IDX_RIGHT_BOT{ 3 };

public:
	static constexpr unsigned NR_VERTICES{ 4 };

	void SetLeftBot(MakerVertex<TVertex> leftBot) { m_Vertices[IDX_LEFT_BOT] = leftBot; }
	void SetLeftTop(MakerVertex<TVertex> leftTop) { m_Vertices[IDX_LEFT_TOP] = leftTop; }
	void SetRightBot(MakerVertex<TVertex> rightBot) { m_Vertices[IDX_RIGHT_BOT] = rightBot; }
	void SetRightTop(MakerVertex<TVertex> rightTop) { m_Vertices[IDX_RIGHT_TOP] = rightTop; }

	void SetLeft(Edge<TVertex> left);
	void SetRight(Edge<TVertex> right);

	MakerVertex<TVertex> GetLeftBot() const { return m_Vertices[IDX_LEFT_BOT]; }
	MakerVertex<TVertex> GetLeftTop() const { return m_Vertices[IDX_LEFT_TOP]; }
	MakerVertex<TVertex> GetRightBot() const { return m_Vertices[IDX_RIGHT_BOT]; }
	MakerVertex<TVertex> GetRightTop() const { return m_Vertices[IDX_RIGHT_TOP]; }

	void SetNormal(const Float3& normal) { m_Normal = normal; }
	const Float3& GetNormal() const { return m_Normal; }

	template<ModelTopology TTopology>
	void CalculateNormal(const MakerData<TVertex, TTopology>& data);

private:
	MakerVertex<TVertex> m_Vertices[NR_VERTICES];
	Float3 m_Normal;
};

template<typename TVertex>
inline void Quad<TVertex>::SetLeft(Edge<TVertex> left)
{
	SetLeftBot(left[0]);
	SetLeftTop(left[1]);
}

template<typename TVertex>
inline void Quad<TVertex>::SetRight(Edge<TVertex> right)
{
	SetRightBot(right[0]);
	SetRightTop(right[1]);
}

template<typename TVertex>
template<ModelTopology TTopology>
inline void Quad<TVertex>::CalculateNormal(const MakerData<TVertex, TTopology>& data)
{
	const Float3 leftBot{ GetLeftBot().GetPosition(data) };
	const Float3 leftTop{ GetLeftTop().GetPosition(data) };
	const Float3 rightBot{ GetRightBot().GetPosition(data) };

	const Float3 up{ leftTop - leftBot };
	const Float3 right{ rightBot - leftBot };

	m_Normal = up.Cross(right).Normalized();
}

}
}