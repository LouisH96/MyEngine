#include "pch.h"
#include "NewUiRectRenderer.h"

#include <Gui\NdcUtils.h>

using namespace NewUi;
using namespace Rendering;

NewUiRectRenderer::NewUiRectRenderer()
	: m_Shader{ Resources::GlobalShader(L"Gui.hlsl") }
	, m_InputLayout{ InputLayout::FromTypes2<Vertex, Instance>() }
	, m_Instances{ 4 }
{
	StackArray<Vertex, 4> vertices{};
	vertices[0].pos = { 0,0 };
	vertices[1].pos = { 0,1 };
	vertices[2].pos = { 1,0 };
	vertices[3].pos = { 1,1 };
	m_Vertices = Buffer<Vertex>{ {vertices }, false };
}

void NewUiRectRenderer::Render()
{
	m_Shader.Activate();
	m_InputLayout.Activate();

	PrimitiveTopology::Activate(TOPOLOGY);
	m_Vertices.Activate(0);
	m_Instances.ActivateVertexBuffer(1);
	m_Instances.Draw(m_Vertices.GetCapacity());
}

void NewUiRectRenderer::Remove(unsigned id)
{
	m_Instances[id].Invalidate();
}

unsigned NewUiRectRenderer::Add(RectFloat rect, const Float3& color)
{
	rect = Gui::NdcUtils::ScreenRectToNdc(rect);

	Instance instance{};
	instance.Position.x = rect.GetLeft();
	instance.Position.y = rect.GetBottom();
	instance.Position.z = .5f;
	instance.Size = rect.GetSize();
	instance.Color = color;

	const unsigned idx{ m_Instances.Add(instance) };
	return idx;
}

void NewUiRectRenderer::SetColor(unsigned id, const Float3& color)
{
	m_Instances[id].Color = color;
}
