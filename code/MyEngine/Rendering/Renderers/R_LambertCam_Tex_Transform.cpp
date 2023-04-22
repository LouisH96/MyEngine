#include "pch.h"
#include "R_LambertCam_Tex_Transform.h"

#include "Framework/Resources.h"
#include "Rendering/State/Mesh.h"
#include "Rendering/State/Texture.h"

Rendering::R_LambertCam_Tex_Transform::R_LambertCam_Tex_Transform(Gpu& gpu, Game::Camera& camera)
	: m_Gpu(gpu)
	, m_Camera(camera)
	, m_BlendState{ gpu }
	, m_RasterizerState{ gpu }
	, m_Sampler{ gpu }
	, m_InputLayout{ gpu, Vertex::ELEMENTS, Vertex::NR_ELEMENTS }
	, m_Shader{ gpu, Framework::Resources::GetGlobalShaderPath(L"lambertCamDir_Texture.hlsl") }
	, m_ConstantBuffer{ gpu }
	, m_Entries{ 0 }
{
}

void Rendering::R_LambertCam_Tex_Transform::Render()
{
	m_Sampler.ActivatePs(m_Gpu);
	m_ConstantBuffer.Activate(m_Gpu);
	m_RasterizerState.Activate(m_Gpu);
	m_InputLayout.Activate(m_Gpu);
	m_BlendState.Activate(m_Gpu);
	m_Shader.Activate();
	for (int i = 0; i < m_Entries.GetSize(); i++)
	{
		m_ConstantBuffer.Update(m_Gpu, CB_CamMatPos{ m_Camera, *m_Entries[i].pTransform });
		m_Entries[i].pTexture->ActivatePs(m_Gpu);
		m_Entries[i].pMesh->Activate(m_Gpu);
		m_Entries[i].pMesh->Draw(m_Gpu);
	}
}

void Rendering::R_LambertCam_Tex_Transform::AddEntry(Mesh& mesh, Texture& texture, Game::Transform& transform)
{
	m_Entries.Add(DrawEntry{ &mesh, &texture, &transform });
}

