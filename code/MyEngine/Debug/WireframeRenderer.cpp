#include "pch.h"
#include "WireframeRenderer.h"
#include <App/Wrappers/Dx/BlendState.h>
#include "App/Wrappers/Dx/RasterizerState.h"
#include <App/Wrappers/Dx/Shader.h>
#include <App/Wrappers/Dx/ConstantBuffer.h>
#include <App/Wrappers/Dx/InputLayout.h>
#include <Game/Camera/Camera.h>
#include "App/Resources.h"

using namespace App::Wrappers::Dx;

Debug::WireframeRenderer::WireframeRenderer(const Gpu& gpu)
{
	InitRenderStates(gpu);
}

Debug::WireframeRenderer::~WireframeRenderer()
{
	delete m_pBlendState;
	delete m_pRasterizerState;
	delete m_pShader;
	delete m_pConstantBuffer;
	delete m_pInputLayout;
	for (int i = 0; i < m_Meshes.GetSize(); i++)
		delete m_Meshes[i];
}

void Debug::WireframeRenderer::AddMesh(const Gpu& gpu,
	const Array<Math::Float3>& points, const Math::Float3& color, const Array<int>& indices)
{
	Array<WireframeVertex> vertices{ points.GetSize() };
	for (int i = 0; i < points.GetSize(); i++)
		vertices[i] = { points[i], color };
	AddMesh(gpu, vertices, indices);
}

void Debug::WireframeRenderer::AddMesh(const Gpu& gpu,
	const Array<WireframeVertex>& vertices, const Array<int>& indices)
{
	Mesh* pMesh = Mesh::Create<WireframeVertex>(gpu, vertices, indices);
	m_Meshes.Add(pMesh);
}

void Debug::WireframeRenderer::Update(const Gpu& gpu, const Game::Camera::Camera& camera) const
{
	m_pConstantBuffer->Update(gpu, { camera.GetViewProjMatrix()});
}

void Debug::WireframeRenderer::Draw(const Gpu& gpu)
{
	if (m_Meshes.GetSize() == 0) return;
	m_pBlendState->Activate(gpu);
	m_pRasterizerState->Activate(gpu);
	m_pInputLayout->Activate(gpu);
	m_pShader->Activate();
	m_pConstantBuffer->Activate(gpu);
	for (int i = 0; i < m_Meshes.GetSize(); i++)
	{
		m_Meshes[i]->Activate();
		m_Meshes[i]->Draw();
	}
}

void Debug::WireframeRenderer::InitRenderStates(const Gpu& gpu)
{
	m_pBlendState = new Dx::BlendState(gpu);
	m_pRasterizerState = new Dx::RasterizerState(gpu, true);
	m_pShader = new Shader(gpu, App::Resources::GetGlobalShaderPath(L"wireframe.hlsl"));
	m_pConstantBuffer = new Dx::ConstantBuffer<ConstantBuffer> (gpu);
	const Dx::InputLayout::Element elements[]
	{
		{"POSITION", Dx::InputLayout::ElementType::Float3},
		{"COLOR", Dx::InputLayout::ElementType::Float3},
	};
	m_pInputLayout = new Dx::InputLayout{ gpu, elements, ARRAYSIZE(elements) };
}
