#include "pch.h"
#include "NodeGraphRenderer.h"

#include "DataStructures/Adders/ArrayAdder.h"
#include "Framework/Resources.h"
#include "Generation/RectGenerator.h"

using namespace Applied;
using namespace Rendering;

NodeGraphRenderer::NodeGraphRenderer()
	: m_InputLayout{ InputLayout::FromType<Vertex>() }
	, m_Shader{ Resources::GlobalShader(L"NodeGraph.hlsl") }
{
	using Gen = RectGenerator<TOPOLOGY>;

	Array<int> indices{ Gen::GetNrIndices() };
	Array<Vertex> vertices{ Gen::GetNrVertices() };
	Gen::GenerateIndices(ArrayAdder<int>{indices}, 0);
	Gen::GenerateVertices([](const Float2& point)
		{
			return Vertex{ point, {1,0,0} };
		}, ArrayAdder<Vertex>{vertices}, RectFloat::FromCenter({ 2,2 }));

	m_Indices = IdxBuffer{ indices };
	m_Vertices = Buffer<Vertex>{ vertices, true };
}

void NodeGraphRenderer::Render(const Camera2D& camera)
{
	m_CameraBuffer.Update(camera.GetCameraBuffer());
	m_CameraBuffer.Activate(0);

	PrimitiveTopology::Activate(TOPOLOGY);
	m_InputLayout.Activate();
	m_Shader.Activate();

	m_Vertices.Activate(0);
	m_Indices.Activate();
	m_Indices.Draw();
}
