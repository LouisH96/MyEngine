#include "pch.h"
#include "R_LambertCam_Tex_Transform.h"

#include "Framework/Resources.h"
#include "Rendering/State/Mesh.h"
#include "Rendering/State/Texture.h"

Rendering::R_LambertCam_Tex_Transform::R_LambertCam_Tex_Transform(Gpu& gpu)
	: m_Gpu(gpu)
	, m_Sampler{ gpu }
	, m_InputLayout{Vertex::ELEMENTS, Vertex::NR_ELEMENTS }
	, m_Shader{ gpu, Framework::Resources::GetGlobalShaderPath(L"LambertCam_Tex_Trans.hlsl") }
	, m_Entries{ 0 }
{
}

void Rendering::R_LambertCam_Tex_Transform::Render(const Math::Float3& cameraPosition, const Math::Float4X4& viewProjection)
{
	using namespace DirectX;
	m_DepthStencilState.Activate();
	m_Sampler.ActivatePs(m_Gpu);
	m_RasterizerState.Activate();
	m_InputLayout.Activate();
	m_BlendState.Activate();
	m_Shader.Activate();
	for (int i = 0; i < m_Entries.GetSize(); i++)
	{
		m_CameraConstantBuffer.Update(CB_CamMatPos{ cameraPosition, viewProjection, *m_Entries[i].pTransform });
		m_CameraConstantBuffer.Activate();
		m_ModelConstantBuffer.Update(CB_ModelBuffer{ *m_Entries[i].pTransform });
		m_ModelConstantBuffer.Activate(1);
		m_Entries[i].pTexture->ActivatePs(m_Gpu);
		m_Entries[i].pMesh->Activate();
		m_Entries[i].pMesh->Draw();
	}
}

void Rendering::R_LambertCam_Tex_Transform::AddEntry(Mesh& mesh, Texture& texture, Game::Transform& transform)
{
	m_Entries.Add(DrawEntry{ &mesh, &texture, &transform });
}

void Rendering::R_LambertCam_Tex_Transform::Remove(const Game::Transform& transform)
{
	for (int i = 0; i < m_Entries.GetSize(); i++)
		if (m_Entries[i].pTransform == &transform)
			m_Entries.RemoveAt(i);
}

void Rendering::R_LambertCam_Tex_Transform::Replace(const Game::Transform& old, Game::Transform& with)
{
	for (int i = 0; i < m_Entries.GetSize(); i++)
		if (m_Entries[i].pTransform == &old)
		{
			m_Entries[i].pTransform = &with;
			return;
		}
}
