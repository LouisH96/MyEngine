#include "pch.h"
#include "UiShapeRenderer.h"

#include <Framework/Resources.h>
#include <Generation\DiskGenerator.h>
#include <Rendering/Canvas.h>

using namespace Ui;
using namespace Rendering;

UiShapeRenderer::UiShapeRenderer()
	: m_Shader{ Resources::GlobalShader(L"Screen_Col.hlsl") }
	, m_InputLayout{ InputLayout::FromType<Vertex>() }
	, m_Vertices{ 32 }
	, m_ToNdcMultiplier{ 2.f / CANVAS.GetSize().x, 2.f / CANVAS.GetSize().y }
{

}

void UiShapeRenderer::OnCanvasResized(const App::ResizedEvent& event)
{
	m_ToNdcMultiplier.x = 2.f / static_cast<float>(event.NewSize.x);
	m_ToNdcMultiplier.y = 2.f / static_cast<float>(event.NewSize.y);
}

void UiShapeRenderer::Render()
{
	PrimitiveTopology::Activate(TOPOLOGY);
	m_InputLayout.Activate();
	m_Shader.Activate();
	m_Vertices.ActivateVertexBuffer(0);
	m_Vertices.Draw();
}

unsigned UiShapeRenderer::Rect(Float2 leftBot, Float2 size, const Float3& color)
{
	leftBot = leftBot * m_ToNdcMultiplier - 1;
	size = size * m_ToNdcMultiplier;

	Shape* pShape;
	const unsigned shapeId{ m_Shapes.Validate(pShape) };

	Vertex v;
	v.col = color;

	//---| Triangle left-bot |---
	//left-bot
	v.pos = leftBot;
	pShape->Triangles.Add(m_Vertices.Add(v));

	//left-top
	v.pos.y += size.y;
	m_Vertices.Add(v);

	//right-bot
	v.pos = leftBot;
	v.pos.x += size.x;
	m_Vertices.Add(v);

	//---| Triangle right-top |---
	//right-bot
	pShape->Triangles.Add(m_Vertices.Add(v));

	//left-top
	v.pos = leftBot;
	v.pos.y += size.y;
	m_Vertices.Add(v);

	//right-top
	v.pos.x += size.x;
	m_Vertices.Add(v);

	return shapeId;
}

unsigned UiShapeRenderer::Line(Float2 begin, Float2 end, float thickness, const Float3& color)
{
	Float2 toEnd{ end - begin };

	const Float2 dir{ toEnd.Normalized() };

	begin = begin * m_ToNdcMultiplier - 1;
	end = end * m_ToNdcMultiplier - 1;
	toEnd *= m_ToNdcMultiplier;

	Float2 right{ dir.y, -dir.x };
	right *= thickness;
	right *= m_ToNdcMultiplier;

	Shape* pShape;
	const unsigned shapeId{ m_Shapes.Validate(pShape) };

	Vertex v;
	v.col = color;

	//---| Left-Bot Triangle |---
	//left-bot
	v.pos = begin - right;
	pShape->Triangles.Add(m_Vertices.Add(v));

	//left-top
	v.pos += toEnd;
	m_Vertices.Add(v);

	//right-bot
	v.pos = begin + right;
	m_Vertices.Add(v);

	//---| Right-Top Triangle |---
	//right-bot
	pShape->Triangles.Add(m_Vertices.Add(v));

	//left-top
	v.pos = begin + toEnd - right;
	m_Vertices.Add(v);

	//right-top
	v.pos += right * 2;
	m_Vertices.Add(v);

	return shapeId;
}

unsigned UiShapeRenderer::Circle(Float2 center, Generation::DiskGenerator<TOPOLOGY>::Options options, const Float3& color)
{
	Shape* pShape;
	const unsigned shapeId{ m_Shapes.Validate(pShape) };

	Adder adder{ *pShape, m_Vertices };

	using Generator = Generation::DiskGenerator<TOPOLOGY>;
	Generator::Generate([center, color, this](const Float2& point)
		{
			return Vertex{ (point + center) * m_ToNdcMultiplier - 1 , color };
		}, adder, options);

	return shapeId;
}

UiShapeRenderer::Adder::Adder(Shape& shape, Rendering::InvalidateBuffer<Vertex>& vertices)
	: m_Shape{ shape }
	, m_Vertices{ vertices }
	, m_NrVertices{ 0 }
{
}

void UiShapeRenderer::Adder::Add(const Vertex& vertex)
{
	if (m_NrVertices++ % 3 == 0)
		m_Shape.Triangles.Add(m_Vertices.Add(vertex));
	else
		m_Vertices.Add(vertex);
}

void UiShapeRenderer::Remove(unsigned id)
{
	Shape& shape{ m_Shapes.Get(id) };

	for (unsigned i = 0; i < shape.Triangles.GetSize(); ++i)
	{
		m_Vertices.Remove(shape.Triangles[i]);
		m_Vertices.Remove(shape.Triangles[i] + 1);
		m_Vertices.Remove(shape.Triangles[i] + 2);
	}

	shape.Invalidate();
}
