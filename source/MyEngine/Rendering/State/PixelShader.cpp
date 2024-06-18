#include "pch.h"
#include "PixelShader.h"

using namespace Rendering;

PixelShader::PixelShader(const std::wstring& path, const std::string& functionName)
	: m_pShader{ nullptr }
{
	Dx::DxHelper::CreatePixelShader(path, functionName, m_pShader);
}

PixelShader::PixelShader(PixelShader&& other) noexcept
	: m_pShader{ other.m_pShader }
{
	other.m_pShader = nullptr;
}

PixelShader& PixelShader::operator=(PixelShader&& other) noexcept
{
	if (&other == this)
		return *this;

	if (m_pShader)m_pShader->Release();
	m_pShader = other.m_pShader;
	other.m_pShader = nullptr;
	return *this;
}

PixelShader::~PixelShader()
{
	if (m_pShader)
		m_pShader->Release();
}

void PixelShader::Activate() const
{
	Globals::pGpu->GetContext().PSSetShader(m_pShader, nullptr, 0);
}
