#include "pch.h"
#include "R_LambertLight_Col.h"

#include "Framework/Resources.h"
#include "Rendering/State/Mesh.h"

Rendering::R_LambertLight_Col::R_LambertLight_Col(Gpu& gpu, bool isWireframe)
	: m_Gpu(gpu)
	, m_RasterizerState{ gpu, isWireframe }
	, m_Sampler{ gpu }
	, m_InputLayout{ gpu, Vertex::ELEMENTS, Vertex::NR_ELEMENTS }
	, m_Shader{ gpu, Framework::Resources::GetGlobalShaderPath(L"LambertLight_Col.hlsl") }
	, m_CameraConstantBuffer{ gpu }
	, m_Entries{ 0 }
{
}

void Rendering::R_LambertLight_Col::Render(const Math::Float3& cameraPosition, const Math::Float4X4& viewProjection)
{
	using namespace DirectX;
	m_DepthStencilState.Activate();
	m_Sampler.ActivatePs(m_Gpu);
	m_RasterizerState.Activate(m_Gpu);
	m_InputLayout.Activate(m_Gpu);
	m_BlendState.Activate();
	m_Shader.Activate();
	for (int i = 0; i < m_Entries.GetSize(); i++)
	{
		m_CameraConstantBuffer.Update(m_Gpu, CB_CamMatPos{ cameraPosition, viewProjection });
		m_CameraConstantBuffer.Activate(m_Gpu);
		m_Entries[i]->Activate(m_Gpu);
		m_Entries[i]->Draw(m_Gpu);
	}
}

void Rendering::R_LambertLight_Col::AddEntry(Mesh& mesh)
{
	m_Entries.Add(&mesh);
}
