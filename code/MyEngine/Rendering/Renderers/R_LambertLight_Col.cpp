#include "pch.h"
#include "R_LambertLight_Col.h"

#include "Framework/Resources.h"
#include "Rendering/State/Mesh.h"

Rendering::R_LambertLight_Col::R_LambertLight_Col(bool isWireframe)
	: m_RasterizerState{ isWireframe }
	, m_InputLayout{ Vertex::ELEMENTS, Vertex::NR_ELEMENTS }
	, m_Shader{ Framework::Resources::GetGlobalShaderPath(L"LambertLight_Col.hlsl") }
	, m_Entries{ 0 }
{
}

void Rendering::R_LambertLight_Col::Render(const Float3& cameraPosition, const Float4X4& viewProjection)
{
	using namespace DirectX;
	m_DepthStencilState.Activate();
	m_Sampler.ActivatePs();
	m_RasterizerState.Activate();
	m_InputLayout.Activate();
	m_BlendState.Activate();
	m_Shader.Activate();
	for (int i = 0; i < m_Entries.GetSize(); i++)
	{
		m_CameraConstantBuffer.Update(CB_CamMatPos{ cameraPosition, viewProjection });
		m_CameraConstantBuffer.Activate();
		m_Entries[i]->Activate();
		m_Entries[i]->Draw();
	}
}

void Rendering::R_LambertLight_Col::AddEntry(Mesh& mesh)
{
	m_Entries.Add(&mesh);
}
