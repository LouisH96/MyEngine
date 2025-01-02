#pragma once

#include <d3d11.h>

namespace MyEngine
{
class ShaderIncludes : public ID3DInclude
{
public:
	ShaderIncludes(const std::wstring& folder);

	HRESULT Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) override;
	HRESULT Close(LPCVOID pData) override;

private:
	std::wstring m_Folder;
};
}