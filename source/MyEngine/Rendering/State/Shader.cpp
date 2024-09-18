#include "pch.h"
#include "Shader.h"

#include "../Dx/DxHelper.h"

using namespace Rendering;

Shader::Shader(Shader&& other) noexcept
	: m_pVertexShader{ other.m_pVertexShader }
	, m_pPixelShader{ other.m_pPixelShader }
{
	other.m_pVertexShader = nullptr;
	other.m_pPixelShader = nullptr;
}

Shader& Shader::operator=(Shader&& other) noexcept
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

Shader::Shader(const std::wstring& fullPath)
{
	InitShaders(fullPath);
}

Shader::~Shader()
{
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexShader);
}

void Shader::ActivateVs() const
{
	Globals::pGpu->GetContext().VSSetShader(m_pVertexShader, nullptr, 0);
}

void Shader::ActivatePs() const
{
	Globals::pGpu->GetContext().PSSetShader(m_pPixelShader, nullptr, 0);
}

void Shader::InitShaders(const std::wstring& fullPath)
{
	Dx::DxHelper::CreateVertexShader(fullPath, "vs_main", m_pVertexShader);
	Dx::DxHelper::CreatePixelShader(fullPath, "ps_main", m_pPixelShader);
}

void Shader::UnsetPs() const
{
	Globals::pGpu->GetContext().PSSetShader(nullptr, nullptr, 0);
}

void Shader::UnsetVs() const
{
	Globals::pGpu->GetContext().VSSetShader(nullptr, nullptr, 0);
}

