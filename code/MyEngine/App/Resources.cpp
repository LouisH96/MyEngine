#include "pch.h"
#include "Resources.h"

#include <Windows.h>

// ReSharper disable once StringLiteralTypo
std::wstring MyEngine::App::Resources::m_GlobalResourcesPath = LR"(D:\VirtualWorld\resources\)";
std::wstring MyEngine::App::Resources::m_LocalResourcesPath = L"";

void MyEngine::App::Resources::Init()
{
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(nullptr, buffer, MAX_PATH);

	const std::wstring cmd{ GetCommandLineW() };
	m_LocalResourcesPath = cmd;

	/*m_GlobalResourcesPath = buffer;
	m_GlobalResourcesPath = m_GlobalResourcesPath.substr(0, m_GlobalResourcesPath.find_last_of('\\'));
	m_GlobalResourcesPath = m_GlobalResourcesPath.append(L"\\resources\\");*/
}

std::wstring MyEngine::App::Resources::GetGlobalResourcePath(const std::wstring& subPath)
{
	return std::wstring(m_GlobalResourcesPath + subPath);
}

std::wstring MyEngine::App::Resources::GetGlobalShaderPath(const std::wstring& subPath)
{
	return std::wstring(m_GlobalResourcesPath + L"Shaders\\" + subPath);
}

std::wstring MyEngine::App::Resources::GetLocalResourcePath(const std::wstring& subPath)
{
	return std::wstring(m_LocalResourcesPath + subPath);
}

std::wstring MyEngine::App::Resources::GetLocalShaderPath(const std::wstring& subPath)
{
	return std::wstring(m_LocalResourcesPath + L"Shaders\\" + subPath);
}
