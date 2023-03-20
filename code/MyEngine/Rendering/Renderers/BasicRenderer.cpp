#include "pch.h"
#include "BasicRenderer.h"

#include "App/Resources.h"
#include "App/Wrappers/Dx/InputLayout.h"
#include "Game/Camera/Camera.h"

using namespace App;
using namespace Wrappers;
using namespace Wrappers::Dx;
using namespace MyEngine::Dx;

const InputLayout::Element Rendering::BasicRenderer::ELEMENTS[] =
{
	{"POSITION", InputLayout::ElementType::Float3},
	{"COLOR", InputLayout::ElementType::Float3},
	{"NORMAL", InputLayout::ElementType::Float3}
};

Rendering::BasicRenderer::BasicRenderer(Gpu& gpu, Game::Camera::Camera& camera)
	: m_Gpu(gpu)
	, m_Camera(camera)
	, m_BlendState{ gpu }
	, m_RasterizerState{ gpu }
	, m_Shader(gpu, Resources::GetGlobalShaderPath(L"lambert.hlsl"))
	, m_InputLayout(m_Gpu, ELEMENTS, ARRAYSIZE(ELEMENTS))
	, m_ConstantBuffer(gpu)
{
}

Rendering::BasicRenderer::~BasicRenderer()
{
	m_Meshes.DeleteAll();
}

void Rendering::BasicRenderer::Render()
{
	m_ConstantBuffer.Update(m_Gpu, { m_Camera.GetViewProjMatrix(), m_Camera.GetPositionFloat3(),0 });
	m_ConstantBuffer.Activate(m_Gpu);
	m_RasterizerState.Activate(m_Gpu);
	m_InputLayout.Activate(m_Gpu);
	m_BlendState.Activate(m_Gpu);
	m_Shader.Activate();
	for (int i = 0; i < m_Meshes.GetSize(); i++)
	{
		m_Meshes[i]->Activate();
		m_Meshes[i]->Draw();
	}
}

void Rendering::BasicRenderer::AddMesh(const Array<Math::Float3>& points, const Array<Math::Float3>& normals,
	const Math::Float3& color, const Array<int>& indices)
{
	Array<MeshVertex> vertices{ points.GetSize() };
	for (int i = 0; i < points.GetSize(); i++)
		vertices[i] = { points[i], color, normals[i]};
	AddMesh(vertices, indices);
}

void Rendering::BasicRenderer::AddMesh(const Array<MeshVertex>& vertices, const Array<int>& indices)
{
	m_Meshes.Add(new Mesh<MeshVertex>( m_Gpu, vertices, indices));
}
