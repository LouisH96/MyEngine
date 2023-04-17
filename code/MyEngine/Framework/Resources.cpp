#include "pch.h"
#include "Resources.h"

#include <Windows.h>

// ReSharper disable once StringLiteralTypo
std::wstring Framework::Resources::m_GlobalResourcesPath = LR"(D:\VirtualWorld\resources\)";
std::wstring Framework::Resources::m_LocalResourcesPath = L"";

void Framework::Resources::Init()
{
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(nullptr, buffer, MAX_PATH);

	int nrArgs;
	LPWSTR* args = CommandLineToArgvW(GetCommandLineW(), &nrArgs);

	if (nrArgs >= 2)
		m_LocalResourcesPath = args[1];
	else
	{
		Logger::PrintError("Local resources path should be defined in args");
		return;
	}

	LocalFree(args);

	/*m_GlobalResourcesPath = buffer;
	m_GlobalResourcesPath = m_GlobalResourcesPath.substr(0, m_GlobalResourcesPath.find_last_of('\\'));
	m_GlobalResourcesPath = m_GlobalResourcesPath.append(L"\\resources\\");*/
}

std::wstring Framework::Resources::GetGlobalResourcePath(const std::wstring& subPath)
{
	return std::wstring(m_GlobalResourcesPath + subPath);
}

std::wstring Framework::Resources::GetGlobalShaderPath(const std::wstring& subPath)
{
	return std::wstring(m_GlobalResourcesPath + L"Shaders\\" + subPath);
}

std::wstring Framework::Resources::GetLocalResourcePath(const std::wstring& subPath)
{
	return std::wstring(m_LocalResourcesPath + subPath);
}

std::wstring Framework::Resources::GetLocalShaderPath(const std::wstring& subPath)
{
	return std::wstring(m_LocalResourcesPath + L"Shaders\\" + subPath);
}
