#include "pch.h"
#include "Shader.h"

#include "DxHelper.h"

App::Wrappers::Dx::Shader::Shader(const Gpu& gpu, const std::wstring& fullPath)
	: m_Gpu(gpu)
{
	InitShaders(fullPath);
}

App::Wrappers::Dx::Shader::~Shader()
{
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexShader);
}

void App::Wrappers::Dx::Shader::Activate() const
{
	m_Gpu.GetContext().VSSetShader(m_pVertexShader, nullptr, 0);
	m_Gpu.GetContext().PSSetShader(m_pPixelShader, nullptr, 0);
}

void App::Wrappers::Dx::Shader::InitShaders(const std::wstring& fullPath)
{
	DxHelper::CreateVertexShader(m_Gpu.GetDevice(), fullPath, "vs_main", m_pVertexShader);
	DxHelper::CreatePixelShader(m_Gpu.GetDevice(), fullPath, "ps_main", m_pPixelShader);
}

