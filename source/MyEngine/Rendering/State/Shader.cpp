#include "pch.h"
#include "Shader.h"

#include "../Dx/DxHelper.h"

Rendering::Shader::Shader(Shader&& other) noexcept
	: m_pVertexShader{ other.m_pVertexShader }
	, m_pPixelShader{ other.m_pPixelShader }
{
	other.m_pVertexShader = nullptr;
	other.m_pPixelShader = nullptr;
}

Rendering::Shader& Rendering::Shader::operator=(Shader&& other) noexcept
{
	if (&other == this) return *this;

	if (m_pPixelShader) m_pPixelShader->Release();
	if (m_pVertexShader) m_pVertexShader->Release();
	m_pPixelShader = other.m_pPixelShader;
	m_pVertexShader = other.m_pVertexShader;
	other.m_pPixelShader = nullptr;
	other.m_pVertexShader = nullptr;
	return *this;
}

Rendering::Shader::Shader(const std::wstring& fullPath)
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
	Globals::pGpu->GetContext().VSSetShader(m_pVertexShader, nullptr, 0);
	Globals::pGpu->GetContext().PSSetShader(m_pPixelShader, nullptr, 0);
}

void Rendering::Shader::InitShaders(const std::wstring& fullPath)
{
	Dx::DxHelper::CreateVertexShader(fullPath, "vs_main", m_pVertexShader);
	Dx::DxHelper::CreatePixelShader(fullPath, "ps_main", m_pPixelShader);
}

