#include "pch.h"
#include "DebugRenderer.h"

#include <Framework/Resources.h>
#include <Generation/Shapes.h>
#include <Rendering/State/Mesh.h>
#include <Rendering/Structs/VertexTypes.h>

DebugRenderer* DebugRenderer::m_pStatic = nullptr;

void DebugRenderer::Init(Rendering::Gpu& gpu)
{
	m_pStatic = new DebugRenderer(gpu);
}

void DebugRenderer::Release()
{
	delete m_pStatic;
}

void DebugRenderer::Render(const Math::Float3& cameraPosition, const Math::Float4X4& viewProjection)
{
	m_pStatic->Class_Render(cameraPosition, viewProjection);
}

void DebugRenderer::AddSphere(const Float3& position, const Float3& color, float radius)
{
	m_pStatic->Class_AddSphere(position, color, radius);
}

void DebugRenderer::AddSpheres(const Array<Float3>& positions, const Float3& color, float radius)
{
	m_pStatic->Class_AddSpheres(positions, color, radius);
}

void DebugRenderer::AddLine(const Math::Float3& begin, const Math::Float3& end, const Math::Float3& color)
{
	m_pStatic->Class_AddLine(begin, end, color);
}

void DebugRenderer::AddLine(const Array<Math::Float3>& points, const Math::Float3& color)
{
	m_pStatic->Class_AddLine(points, color);
}

void DebugRenderer::AddRay(const Math::Float3& origin, const Math::Float3& displacement, const Math::Float3& color)
{
	Array<LineVertex> vertices{ 2 };
	vertices[0] = { origin, color };
	vertices[1] = { origin + displacement, color };
	m_pStatic->m_pLineRenderer->AddMesh(Rendering::Mesh::Create(m_pStatic->m_Gpu, vertices, Rendering::Topology::LineStrip));
}

void DebugRenderer::AddRect(const Math::Float3& leftTop, const Math::Float3& rightTop, const Math::Float3& rightBot,
	const Math::Float3& leftBot, const Math::Float3& color)
{
	m_pStatic->Class_AddRect(leftTop, rightTop, rightBot, leftBot, color);
}

void DebugRenderer::AddGridXy(const Math::Float3& offset, const Math::Float2& bounds, int nrCols, int nrRows,
	const Math::Float3& color)
{
	const float bot{ offset.y };
	const float top{ bot + bounds.y };
	const float left{ offset.x };
	const float right{ left + bounds.x };
	const float colWidth{ bounds.x / nrCols };
	const float rowHeight{ bounds.y / nrRows };

	Array<LineVertex> vertices{ 4 + (nrCols + nrRows) * 2 };
	int iVertex = 0;
	LineVertex vertex{ {left, bot, offset.z}, color };

	//VERTICAL
	vertices[iVertex++] = vertex; //first left-bot
	for (int i = 0; i < nrCols;)
	{
		if (i % 2 == 0) //left-top & right-top
		{
			//left-top
			vertex.Pos.y = top;
			vertices[iVertex++] = vertex;
			//right-top
			vertex.Pos.x = left + ++i * colWidth;
			vertices[iVertex++] = vertex;
		}
		else //left-bot & right-bot
		{
			//left-bot
			vertex.Pos.y = bot;
			vertices[iVertex++] = vertex;
			//right-bot
			vertex.Pos.x = left + ++i * colWidth;
			vertices[iVertex++] = vertex;
		}
	}
	if (nrCols % 2 == 0) //right-top
		vertex.Pos.y = top;
	else
		vertex.Pos.y = bot; //right-bot
	vertices[iVertex++] = vertex;

	//HORIZONTAL
	vertex.Pos.x = right;
	vertex.Pos.y = bot;
	vertices[iVertex++] = vertex; //first right-bot
	for (int i = 0; i < nrRows;)
	{
		if (i % 2 == 0) //left-bot & left-top
		{
			//bot-left
			vertex.Pos.x = left;
			vertices[iVertex++] = vertex;
			//top-left
			vertex.Pos.y = bot + ++i * rowHeight;
			vertices[iVertex++] = vertex;
		}
		else //right-bot & right-top
		{
			//right-bot
			vertex.Pos.x = right;
			vertices[iVertex++] = vertex;
			//right-top
			vertex.Pos.y = bot + ++i * rowHeight;
			vertices[iVertex++] = vertex;
		}
	}
	if (nrRows % 2 == 0)
		vertex.Pos.x = left;
	else
		vertex.Pos.x = right;
	vertices[iVertex] = vertex;
	m_pStatic->m_pLineRenderer->AddMesh(Rendering::Mesh::Create(m_pStatic->m_Gpu, vertices, Rendering::Topology::LineStrip));
}

DebugRenderer::DebugRenderer(Rendering::Gpu& gpu)
	: m_Gpu(gpu)
	, m_BlendState(gpu)
	, m_RasterizerState(gpu)
	, m_InputLayout(gpu, Vertex::ELEMENTS, Vertex::NR_ELEMENTS)
	, m_ConstantBuffer(gpu)
	, m_Shader(gpu, Framework::Resources::GetGlobalShaderPath(L"lambertCamDir.hlsl"))
	, m_pLineRenderer(Rendering::RendererFactory::CreateUnlitRenderer(gpu))
	, m_SphereInstancingRenderer{ gpu }
{
}

DebugRenderer::~DebugRenderer()
{
	m_Meshes.DeleteAll();
	delete m_pLineRenderer;
}

void DebugRenderer::Class_Render(const Math::Float3& cameraPosition, const Math::Float4X4& viewProjection)
{
	m_ConstantBuffer.Update(m_Gpu, Rendering::CB_CamMatPos{ cameraPosition, viewProjection });
	m_ConstantBuffer.Activate(m_Gpu);
	m_RasterizerState.Activate(m_Gpu);
	m_InputLayout.Activate(m_Gpu);
	m_BlendState.Activate(m_Gpu);
	m_Shader.Activate();
	for (int i = 0; i < m_Meshes.GetSize(); i++)
	{
		m_Meshes[i]->Activate(m_Gpu);
		m_Meshes[i]->Draw(m_Gpu);
	}
	m_SphereInstancingRenderer.Render(m_Gpu, cameraPosition, viewProjection);
	m_pLineRenderer->Render(cameraPosition, viewProjection);
}

void DebugRenderer::Class_AddSphere(const Float3& position, const Float3& color, float radius)
{
	const Sphere sphere{ position, radius };
	Array<Float3> positions{};
	Array<Float3> normals{};
	Array<int> indices{};
	Generation::Shapes::GenerateSphereBuffers(sphere, 10, 10, positions, normals, indices);
	Array<Vertex> vertices{ positions.GetSize() };
	for (int i = 0; i < positions.GetSize(); i++)
		vertices[i] = Vertex{ positions[i], color, normals[i] };
	m_Meshes.Add(Rendering::Mesh::Create<Vertex>(m_Gpu, vertices, indices));
}

void DebugRenderer::Class_AddSpheres(const Array<Float3>& positions, const Float3& color, float radius)
{
	//todo: all these spheres should be 1 mesh
	for (int i = 0; i < positions.GetSize(); i++)
		Class_AddSphere(positions[i], color, radius);
}

void DebugRenderer::Class_AddLine(const Math::Float3& begin, const Math::Float3& end, const Math::Float3& color) const
{
	Array<LineVertex> vertices{ 2 };
	vertices[0] = { begin, color };
	vertices[1] = { end, color };
	m_pLineRenderer->AddMesh(Rendering::Mesh::Create(m_Gpu, vertices, Rendering::Topology::LineStrip));
}

void DebugRenderer::Class_AddLine(const Array<Math::Float3>& points, const Math::Float3& color) const
{
	Array<LineVertex> vertices{ points.GetSize() };
	for (int i = 0; i < points.GetSize(); i++)
		vertices[i] = { points[i], color };
	m_pLineRenderer->AddMesh(Rendering::Mesh::Create(m_Gpu, vertices, Rendering::Topology::LineStrip));
}

void DebugRenderer::Class_AddRect(const Math::Float3& leftTop, const Math::Float3& rightTop,
	const Math::Float3& rightBot, const Math::Float3& leftBot, const Math::Float3& color) const
{
	Array<LineVertex> vertices{ 5 };
	vertices[0] = { leftTop, color }; vertices[1] = { rightTop, color };
	vertices[2] = { rightBot, color }; vertices[3] = { leftBot, color };
	vertices[4] = vertices[0];
	m_pLineRenderer->AddMesh(Rendering::Mesh::Create(m_Gpu, vertices, Rendering::Topology::LineStrip));
}
