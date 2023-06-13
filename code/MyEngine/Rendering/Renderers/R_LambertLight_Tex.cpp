#include "pch.h"
#include "R_LambertLight_Tex.h"

#include <Framework/Resources.h>
#include <Rendering/State/Mesh.h>
#include <Rendering/State/Texture.h>

Rendering::R_LambertLight_Tex::R_LambertLight_Tex(Gpu& gpu)
	: m_Gpu(gpu)
	, m_InputLayout{ Vertex::ELEMENTS, Vertex::NR_ELEMENTS }
	, m_Shader{ Framework::Resources::GetGlobalShaderPath(L"LambertLight_Tex.hlsl") }
	, m_Entries{ 0 }
{
}

void Rendering::R_LambertLight_Tex::Render(const Math::Float3& cameraPosition, const Math::Float4X4& viewProjection)
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
		m_Entries[i].pTexture->ActivatePs();
		m_Entries[i].pMesh->Activate();
		m_Entries[i].pMesh->Draw();
	}
}

void Rendering::R_LambertLight_Tex::AddEntry(Mesh& mesh, Texture& texture)
{
	m_Entries.Add(DrawEntry{ &mesh, &texture });
}
