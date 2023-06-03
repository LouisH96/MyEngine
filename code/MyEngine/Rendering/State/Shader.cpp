#include "pch.h"
#include "Shader.h"

#include "../Dx/DxHelper.h"

Rendering::Shader::Shader(const std::wstring& fullPath)
	: Shader{ *Globals::pGpu, fullPath }
{
}

Rendering::Shader::Shader(const Gpu& gpu, const std::wstring& fullPath)
	: m_Gpu(gpu)
{
	InitShaders(fullPath);
}

Rendering::Shader::~Shader()
{
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexShader);
}

void Rendering::Shader::Activate() const
{
	m_Gpu.GetContext().VSSetShader(m_pVertexShader, nullptr, 0);
	m_Gpu.GetContext().PSSetShader(m_pPixelShader, nullptr, 0);
}

void Rendering::Shader::InitShaders(const std::wstring& fullPath)
{
	Dx::DxHelper::CreateVertexShader(m_Gpu.GetDevice(), fullPath, "vs_main", m_pVertexShader);
	Dx::DxHelper::CreatePixelShader(m_Gpu.GetDevice(), fullPath, "ps_main", m_pPixelShader);
}

