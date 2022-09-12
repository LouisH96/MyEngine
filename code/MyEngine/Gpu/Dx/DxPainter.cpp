#include "DxPainter.h"

#include "DxCanvas.h"
#include "DxShader.h"
#include "DxMesh.h"

void MyEngine::Gpu::Dx::DxPainter::SetCanvas(ICanvas& canvas)
{
	m_pCanvas = reinterpret_cast<DxCanvas*>(&canvas);
	m_pCanvas->Activate();
}

void MyEngine::Gpu::Dx::DxPainter::SetShader(IShader& shader)
{
	m_pShader = reinterpret_cast<DxShader*>(&shader);
	m_pShader->Activate();

	if (m_IsPainting && m_pCamera)
		m_pShader->OnCamUpdated(*m_pCamera);
}

void MyEngine::Gpu::Dx::DxPainter::SetMesh(IMesh& mesh)
{
	m_pMesh = reinterpret_cast<DxMesh*>(&mesh);
	m_pMesh->Activate();
}

void MyEngine::Gpu::Dx::DxPainter::SetCamera(Game::Camera::ICamera& camera)
{
	//Changing camera while painting won't work
	m_pCamera = &camera;
}

void MyEngine::Gpu::Dx::DxPainter::OnCamUpdated()
{
	if (m_pShader)
		m_pShader->OnCamUpdated(*m_pCamera);
}

void MyEngine::Gpu::Dx::DxPainter::BeginPaint()
{
	OnCamUpdated();
	m_pCanvas->Clear();
	m_pCanvas->Activate();
}

void MyEngine::Gpu::Dx::DxPainter::EndPaint()
{
	m_pCanvas->Present();
}

void MyEngine::Gpu::Dx::DxPainter::Paint() const
{
	m_pMesh->Draw();
}
