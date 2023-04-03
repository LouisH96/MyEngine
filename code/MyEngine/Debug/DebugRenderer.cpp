#include "pch.h"
#include "DebugRenderer.h"

#include "App/Resources.h"
#include <Rendering/State/Mesh.h>
#include "Math/Sphere.h"
#include "Rendering/Structs/VertexTypes.h"
#include <Generation/Shapes.h>

DebugRenderer* DebugRenderer::m_pStatic = nullptr;

void DebugRenderer::Init(Rendering::Gpu& gpu, Game::Camera& camera)
{
	m_pStatic = new DebugRenderer(gpu, camera);
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

void DebugRenderer::AddLine(const Math::Float3& begin, const Math::Float3& end, const Math::Float3& color)
{
	m_pStatic->Class_AddLine(begin, end, color);
}

void DebugRenderer::AddLine(const Array<Math::Float3>& points, const Math::Float3& color)
{
	m_pStatic->Class_AddLine(points, color);
}

DebugRenderer::DebugRenderer(Rendering::Gpu& gpu, Game::Camera& camera)
	: m_Gpu(gpu)
	, m_Camera(camera)
	, m_BlendState(gpu)
	, m_RasterizerState(gpu)
	, m_InputLayout(gpu, Vertex::ELEMENTS, Vertex::NR_ELEMENTS)
	, m_ConstantBuffer(gpu)
	, m_Shader(gpu, App::Resources::GetGlobalShaderPath(L"lambertCamDir.hlsl"))
	, m_pLineRenderer(Rendering::RendererFactory::CreateUnlitRenderer(gpu, camera))
{
}

DebugRenderer::~DebugRenderer()
{
	m_Meshes.DeleteAll();
	delete m_pLineRenderer;
}

void DebugRenderer::Class_Render()
{
	m_ConstantBuffer.Update(m_Gpu, { m_Camera });
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
	m_pLineRenderer->Render();
}

void DebugRenderer::Class_AddSphere(const Float3& position, const Float3& color, float radius)
{
	const Sphere sphere{ position, radius };
	Array<Float3> positions{};
	Array<Float3> normals{};
	Array<int> indices{};
	Generation::Shapes::GenerateSphereBuffers(sphere, 5, 5, positions, normals, indices);
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
