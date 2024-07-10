#include "pch.h"
#include "R_LambertCam_Tex_Tran_Inst.h"

#include "Framework/Resources.h"
#include "Camera/Camera.h"

Rendering::R_LambertCam_Tex_Tran_Inst::R_LambertCam_Tex_Tran_Inst()
	: m_InputLayout{ InputLayout::FromTypes<Vertex, Instance>() }
	, m_Shader{ Resources::GlobalShader(L"LambertCam_Tex_Tran_Inst.hlsl") }
{
}

void Rendering::R_LambertCam_Tex_Tran_Inst::Render()
{
	m_BlendState.Activate();
	m_RasterizerState.Activate();
	m_Sampler.Activate();
	m_DepthStencilState.Activate();
	m_InputLayout.Activate();
	m_Shader.Activate();
	m_CameraBuffer.Update(CB_CamPos{ Globals::pCamera->GetPosition() });
	m_CameraBuffer.Activate();

	for (int iModel = 0; iModel < m_InstanceLists.GetSizeS(); iModel++)
	{
		m_Textures[iModel].ActivatePs();
		const List<const Transform*>& transformList{m_Transforms[iModel]};
		InstanceList<Vertex, Instance>& instanceList{m_InstanceLists[iModel]};
		for (int iInstance = 0; iInstance < transformList.GetSizeS(); iInstance++)
		{
			const Transform& transform{ *transformList[iInstance] };
			Instance& instance{ instanceList[iInstance] };
			instance.model = transform.AsMatrix();
			instance.modelViewProj = instance.model * Globals::pCamera->GetViewProjection();
		}
		instanceList.Draw();
	}
}

int Rendering::R_LambertCam_Tex_Tran_Inst::CreateModel(Texture&& texture, const Vertex* pVertices, unsigned nrVertices)
{
	m_Textures.Add(std::move(texture));
	m_InstanceLists.Add({ pVertices, nrVertices });
	return m_InstanceLists.GetSizeS() - 1;
}

void Rendering::R_LambertCam_Tex_Tran_Inst::AddInstance(int modelIdx, const Transform& instanceTransform)
{
	m_InstanceLists[modelIdx].Add({});
	m_Transforms[modelIdx].Add(&instanceTransform);
}
