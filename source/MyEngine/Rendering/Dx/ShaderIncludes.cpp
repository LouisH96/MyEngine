#include "pch.h"
#include "ShaderIncludes.h"

#include <FileUtils\FileUtils.h>
#include <fstream>

//https://github.com/TheRealMJP/BakingLab/blob/master/SampleFramework11/v1.02/Graphics/ShaderCompilation.cpp#L131

// Converts an ANSI string to a std::wstring
inline std::wstring AnsiToWString(const char* ansiString)
{
	wchar_t buffer[512];
	MultiByteToWideChar(CP_ACP, 0, ansiString, -1, buffer, 512);
	return std::wstring(buffer);
}

ShaderIncludes::ShaderIncludes(const std::wstring& filePath)
	: m_Folder{ FileUtils::GetFolder(filePath) }
{

}

HRESULT ShaderIncludes::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID, LPCVOID* ppData, UINT* pBytes)
{
	std::wstring filePath;
	if (IncludeType == D3D_INCLUDE_LOCAL)
		filePath = m_Folder + AnsiToWString(pFileName);
	else if (IncludeType & D3D_INCLUDE_SYSTEM)
		filePath = Resources::GlobalShader(AnsiToWString(pFileName));
	else
		return E_FAIL;

	std::ifstream f{ filePath, std::ios::binary };

	if (!f.is_open())
		Logger::PrintError("[ShaderIncludes] could not open file");

	const std::streampos begin{ f.tellg() };
	f.seekg(0, std::ios::end);
	const std::streampos end{ f.tellg() };
	*pBytes = UINT(end - begin);
	f.seekg(0, std::ios::beg);

	char* data = reinterpret_cast<char*>(std::malloc(*pBytes));
	f.read(data, *pBytes);
	*ppData = data;
	return S_OK;
}

HRESULT ShaderIncludes::Close(LPCVOID pData)
{
	std::free(const_cast<void*>(pData));
	return S_OK;
}
