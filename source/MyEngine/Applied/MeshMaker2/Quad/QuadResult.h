#pragma once
#include <Geometry\ModelTopology.h>
#include "..\MakerResult.h"
#include "..\MakerVertex.h"

namespace MyEngine
{
namespace MeshMaker2
{

template<typename TVertex, ModelTopology TTopology>
class EmptyQuadResult
	: public MakerResult<TVertex, TTopology>
{
public:
	void AddLeftBot(int) {};
	void AddLeftTop(int) {};
	void AddRightBot(int) {};
	void AddRightTop(int) {};
};

template<typename TVertex, ModelTopology TTopology>
class QuadResult
	: public MakerResult<TVertex, TTopology>
{
private:
	using BaseQuad = Quad<TVertex>;

public:
	void AddLeftBot(int leftBot) { m_Vertices[BaseQuad::IDX_LEFT_BOT] = leftBot; }
	void AddLeftTop(int leftTop) { m_Vertices[BaseQuad::IDX_LEFT_TOP] = leftTop; }
	void AddRightBot(int rightBot) { m_Vertices[BaseQuad::IDX_RIGHT_BOT] = rightBot; }
	void AddRightTop(int rightTop) { m_Vertices[BaseQuad::IDX_RIGHT_TOP] = rightTop; }

	RefMakerVertex<TVertex> GetLeftBot() const { return { m_Vertices[BaseQuad::IDX_LEFT_BOT] }; }
	RefMakerVertex<TVertex> GetLeftTop() const { return { m_Vertices[BaseQuad::IDX_LEFT_TOP] }; }
	RefMakerVertex<TVertex> GetRightBot() const { return { m_Vertices[BaseQuad::IDX_RIGHT_BOT] }; }
	RefMakerVertex<TVertex> GetRightTop() const { return { m_Vertices[BaseQuad::IDX_RIGHT_TOP] }; }

private:
	int m_Vertices[BaseQuad::NR_VERTICES];
};

}
}