#include "Resources.h"

#include <Windows.h>

std::wstring MyEngine::App::Resources::m_ResourceFolderPath;

void MyEngine::App::Resources::Init()
{
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(nullptr, buffer, MAX_PATH);

	m_ResourceFolderPath = buffer;
	m_ResourceFolderPath = m_ResourceFolderPath.substr(0, m_ResourceFolderPath.find_last_of('\\'));
	m_ResourceFolderPath = m_ResourceFolderPath.append(L"\\resources\\");
}

std::wstring MyEngine::App::Resources::GetResourcePath(const std::wstring& subPath)
{
	return std::wstring(m_ResourceFolderPath + subPath);
}

std::wstring MyEngine::App::Resources::GetShaderPath(const std::wstring& subPath)
{
	return std::wstring(m_ResourceFolderPath + L"Shaders\\" + subPath);
}
