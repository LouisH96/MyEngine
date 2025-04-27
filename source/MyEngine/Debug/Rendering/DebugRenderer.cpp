#include "pch.h"

#include "DebugRenderer.h"
#include <Framework/Resources.h>
#include <Generation/Shapes.h>
#include <Geometry/Shapes/Sphere.h>
#include <Rendering/State/Mesh.h>
#include <Rendering/Structs/VertexTypes.h>

using namespace Rendering;

DebugRenderer* DebugRenderer::m_pStatic = nullptr;

void DebugRenderer::Init()
{
	if (m_pStatic) Logger::Error("Static DebugRenderer already assigned");
	m_pStatic = new DebugRenderer();
}

void DebugRenderer::Release()
{
	delete m_pStatic;
}

void DebugRenderer::Render()
{
	m_pStatic->Class_Render();
}

void DebugRenderer::AddSphere(const Float3& position, const Float3& color, float radius)
{
	m_pStatic->Class_AddSphere(position, color, radius);
}

void DebugRenderer::AddSpheres(const Array<Float3>& positions, const Float3& color, float radius)
{
	m_pStatic->Class_AddSpheres(positions, color, radius);
}

void DebugRenderer::AddLine(const Float3& begin, const Float3& end, const Float3& color)
{
	m_pStatic->Class_AddLine(begin, end, color);
}

void DebugRenderer::AddLine(const Array<Float3>& points, const Float3& color)
{
	m_pStatic->Class_AddLine(points, color);
}

void DebugRenderer::DrawLine(const Float3& begin, const Float3& end, const Float3& color)
{
	m_pStatic->m_LinesRenderer2.DrawLine(begin, end, color);
}

void DebugRenderer::DrawLine(const Line& line, const Float3& color)
{
	m_pStatic->m_LinesRenderer2.DrawLine(line, color);
}

void DebugRenderer::DrawLine(PtrRangeConst<Float3> points, const Float3& color)
{
	m_pStatic->m_LinesRenderer2.DrawLine(points.pData, points.count, color);
}

void DebugRenderer::DrawRay(const Float3& origin, const Float3& displacement, const Float3& color)
{
	m_pStatic->m_LinesRenderer2.DrawRay(origin, displacement, color);
}

void DebugRenderer::DrawRay(const Ray& ray, const Float3& color)
{
	m_pStatic->m_LinesRenderer2.DrawRay(ray, color);
}

void DebugRenderer::DrawTriangle(const Triangle& triangle, const Float3& color)
{
	m_pStatic->m_TriangleRenderer.Draw(triangle, color);
}

void DebugRenderer::DrawTriangle(const Float3* pTriangle, const Float3& color)
{
	m_pStatic->m_TriangleRenderer.Draw(pTriangle, color);
}

void DebugRenderer::DrawTriangle(const Float3* pTriangle, const Float3& normal, const Float3& color)
{
	m_pStatic->m_TriangleRenderer.Draw(pTriangle, normal, color);
}

void DebugRenderer::AddRay(const Float3& origin, const Float3& displacement, const Float3& color)
{
	Array<LineVertex> vertices{ 2 };
	vertices[0] = { origin, color };
	vertices[1] = { origin + displacement, color };
	m_pStatic->m_pLineRenderer->AddMesh(Mesh::Create(vertices, Topology::LineStrip));
}

void DebugRenderer::AddRect(const Float3& leftTop, const Float3& rightTop, const Float3& rightBot,
	const Float3& leftBot, const Float3& color)
{
	m_pStatic->Class_AddRect(leftTop, rightTop, rightBot, leftBot, color);
}

void DebugRenderer::AddGridXy(const Float3& offset, const Float2& bounds, int nrCols, int nrRows,
	const Float3& color)
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
	m_pStatic->m_pLineRenderer->AddMesh(Mesh::Create(vertices, Topology::LineStrip));
}

void DebugRenderer::DrawSphere(const Float3& position, const Float3& color, float radius)
{
	m_pStatic->m_SpheresRenderer.DrawSphere(position, color, radius);
}

void DebugRenderer::DrawSphere(const Sphere& sphere, const Float3& color)
{
	m_pStatic->m_SpheresRenderer.DrawSphere(sphere, color);
}

void DebugRenderer::DrawDiskXz(const Float3& center, float radius, const Float3& color)
{
	m_pStatic->m_LinesRenderer2.DrawDiskXz(center, radius, color);
}

void DebugRenderer::DrawCube(const Cube& cube, const Float3& color)
{
	m_pStatic->m_CubeRenderer.Draw(cube, color);
}

void DebugRenderer::DrawCube(const CubeAA& cube, const Float3& color)
{
	m_pStatic->m_CubeRenderer.Draw(cube, color);
}

void DebugRenderer::DrawCone(const Cone& cone, const Float3& color)
{
	m_pStatic->m_ConeRenderer.Draw(cone, color);
}

DebugRenderer::DebugRenderer()
	: m_InputLayout(Vertex::ELEMENTS, Vertex::NR_ELEMENTS)
	, m_Shader(Resources::GlobalShader(L"lambertCamDir.hlsl"))
	, m_pLineRenderer(RendererFactory::CreateUnlitRenderer())
{
}

DebugRenderer::~DebugRenderer()
{
	m_Meshes.DeleteAll();
	delete m_pLineRenderer;
}

void DebugRenderer::Class_Render()
{
	m_pLineRenderer->Render();
	m_LinesRenderer2.Render();

	m_DepthStencilState.Activate();
	m_ConstantBuffer.Update(CB_CamMatPos{ Globals::pCamera->GetPosition(), Globals::pCamera->GetViewProjection()});
	m_ConstantBuffer.Activate();
	m_RasterizerState.Activate();
	m_InputLayout.Activate();
	m_BlendState.Activate();
	m_Shader.Activate();
	for (unsigned i = 0; i < m_Meshes.GetSize(); i++)
	{
		m_Meshes[i]->Activate();
		m_Meshes[i]->Draw();
	}
	m_CubeRenderer.Render();
	m_ConeRenderer.Render();

	PrimitiveTopology::Activate(ModelTopology::TriangleList);
	m_TriangleRenderer.Render();

	m_SpheresRenderer.Render();
}

void DebugRenderer::Class_AddSphere(const Float3& position, const Float3& color, float radius)
{
	const Sphere sphere{ position, radius };
	Array<Float3> positions{};
	Array<Float3> normals{};
	Array<int> indices{};
	Generation::Shapes::GenerateSphereBuffers(sphere, 10, 10, positions, normals, indices);
	Array<Vertex> vertices{ positions.GetSize() };
	for (unsigned i = 0; i < positions.GetSize(); i++)
		vertices[i] = Vertex{ positions[i], color, normals[i] };
	m_Meshes.Add(Mesh::Create<Vertex>(vertices, indices));
}

void DebugRenderer::Class_AddSpheres(const Array<Float3>& positions, const Float3& color, float radius)
{
	//todo: all these spheres should be 1 mesh
	for (unsigned i = 0; i < positions.GetSize(); i++)
		Class_AddSphere(positions[i], color, radius);
}

void DebugRenderer::Class_AddLine(const Float3& begin, const Float3& end, const Float3& color) const
{
	Array<LineVertex> vertices{ 2 };
	vertices[0] = { begin, color };
	vertices[1] = { end, color };
	m_pLineRenderer->AddMesh(Mesh::Create(vertices, Topology::LineStrip));
}

void DebugRenderer::Class_AddLine(const Array<Float3>& points, const Float3& color) const
{
	Array<LineVertex> vertices{ points.GetSize() };
	for (unsigned i = 0; i < points.GetSize(); i++)
		vertices[i] = { points[i], color };
	m_pLineRenderer->AddMesh(Mesh::Create(vertices, Topology::LineStrip));
}

void DebugRenderer::Class_AddRect(const Float3& leftTop, const Float3& rightTop,
	const Float3& rightBot, const Float3& leftBot, const Float3& color) const
{
	Array<LineVertex> vertices{ 5 };
	vertices[0] = { leftTop, color }; vertices[1] = { rightTop, color };
	vertices[2] = { rightBot, color }; vertices[3] = { leftBot, color };
	vertices[4] = vertices[0];
	m_pLineRenderer->AddMesh(Mesh::Create(vertices, Topology::LineStrip));
}
