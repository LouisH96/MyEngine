#pragma once

#include "..\MakerVertex.h"

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

	MakerVertex<TVertex> GetLeftBot() const { return m_Vertices[IDX_LEFT_BOT]; }
	MakerVertex<TVertex> GetLeftTop() const { return m_Vertices[IDX_LEFT_TOP]; }
	MakerVertex<TVertex> GetRightBot() const { return m_Vertices[IDX_RIGHT_BOT]; }
	MakerVertex<TVertex> GetRightTop() const { return m_Vertices[IDX_RIGHT_TOP]; }

	void SetNormal(const Float3& normal) { m_Normal = normal; }
	const Float3& GetNormal() const { return m_Normal; }

private:
	MakerVertex<TVertex> m_Vertices[NR_VERTICES];
	Float3 m_Normal;
};

}
}