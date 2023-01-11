#include "pch.h"
#include "Painter.h"

#include "Canvas.h"
#include "Shader.h"
#include "Mesh.h"

void MyEngine::Wrappers::Gpu::Painter::SetCanvas(Canvas& canvas)
{
	m_pCanvas = reinterpret_cast<Canvas*>(&canvas);
	m_pCanvas->Activate();
}

void MyEngine::Wrappers::Gpu::Painter::SetShader(Shader& shader)
{
	m_pShader = reinterpret_cast<Shader*>(&shader);
	m_pShader->Activate();

	if (m_IsPainting && m_pCamera)
		m_pShader->OnCamUpdated(*m_pCamera);
}

void MyEngine::Wrappers::Gpu::Painter::SetMesh(Mesh& mesh)
{
	m_pMesh = reinterpret_cast<Mesh*>(&mesh);
	m_pMesh->Activate();
}

void MyEngine::Wrappers::Gpu::Painter::SetCamera(Game::Camera::Camera& camera)
{
	//Changing camera while painting won't work
	m_pCamera = &camera;
}

void MyEngine::Wrappers::Gpu::Painter::OnCamUpdated()
{
	if (m_pShader)
		m_pShader->OnCamUpdated(*m_pCamera);
}

void MyEngine::Wrappers::Gpu::Painter::BeginPaint()
{
	OnCamUpdated();
	m_pCanvas->Clear();
	m_pCanvas->Activate();
}

void MyEngine::Wrappers::Gpu::Painter::EndPaint()
{
	m_pCanvas->Present();
}

void MyEngine::Wrappers::Gpu::Painter::Paint() const
{
	m_pMesh->Draw();
}
