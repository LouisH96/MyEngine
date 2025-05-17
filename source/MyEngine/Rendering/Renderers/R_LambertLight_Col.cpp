#include "pch.h"
#include "R_LambertLight_Col.h"

#include "Framework/Resources.h"
#include "Camera/Camera.h"
#include "Rendering/State/Mesh.h"

using namespace Rendering;

R_LambertLight_Col::R_LambertLight_Col(bool isWireframe)
	: m_RasterizerState{ RasterizerState::MakeDefault(isWireframe) }
	, m_InputLayout{ Vertex::ELEMENTS, Vertex::NR_ELEMENTS }
	, m_Shader{ Resources::GlobalShader(L"LambertLight_Col.hlsl") }
	, m_Entries{ 0 }
{
}

void R_LambertLight_Col::Render_Internal()
{
	using namespace DirectX;
	m_DepthStencilState.Activate();
	m_Sampler.ActivatePs();
	m_RasterizerState.Activate();
	m_InputLayout.Activate();
	m_BlendState.Activate();
	for (unsigned i = 0; i < m_Entries.GetSize(); i++)
	{
		m_CameraConstantBuffer.Update(CB_CamMatPos{ Globals::pCamera->GetPosition(), Globals::pCamera->GetViewProjection() });
		m_CameraConstantBuffer.Activate();
		m_Entries[i]->Activate();
		m_Entries[i]->Draw();
	}
}

void R_LambertLight_Col::AddEntry(Mesh& mesh)
{
	m_Entries.Add(&mesh);
}
