#include "pch.h"
#include "PivotRenderer.h"

#include "Framework/Resources.h"
#include "Generation/Shapes/ArrowGenerator.h"

using namespace Applied;
using namespace Rendering;

PivotRenderer::PivotRenderer()
	: m_Shader{ Resources::GlobalShader(L"lambertCamDir.hlsl") }
	, m_InputLayout{InputLayout::FromType<Vertex>() }
{
	Array<Vertex> pivotVertices{};
	Array<int> pivotIndices{};
	Generation::ArrowGenerator::CreatePivotArrows(pivotVertices, pivotIndices, 16, { 0,0,0 });

	m_Vertices = Buffer<Vertex>{ {pivotVertices}, false };
	m_Indices = IdxBuffer{ {pivotIndices} };
}

void PivotRenderer::Render()
{
	m_Shader.Activate();
	m_InputLayout.Activate();

	m_Vertices.Activate(0);
	m_Indices.Activate();
	m_Indices.Draw();
}
