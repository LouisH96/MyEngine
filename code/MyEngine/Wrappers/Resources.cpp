#include "pch.h"
#include "Resources.h"

#include <Windows.h>

// ReSharper disable once StringLiteralTypo
std::wstring MyEngine::Wrappers::Resources::m_ResourceFolderPath = LR"(D:\VirtualWorld\resources\)";

void MyEngine::Wrappers::Resources::Init()
{
	/*TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(nullptr, buffer, MAX_PATH);

	m_ResourceFolderPath = buffer;
	m_ResourceFolderPath = m_ResourceFolderPath.substr(0, m_ResourceFolderPath.find_last_of('\\'));
	m_ResourceFolderPath = m_ResourceFolderPath.append(L"\\resources\\");*/
}

std::wstring MyEngine::Wrappers::Resources::GetResourcePath(const std::wstring& subPath)
{
	return std::wstring(m_ResourceFolderPath + subPath);
}

std::wstring MyEngine::Wrappers::Resources::GetShaderPath(const std::wstring& subPath)
{
	return std::wstring(m_ResourceFolderPath + L"Shaders\\" + subPath);
}
