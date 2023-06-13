#include "pch.h"
#include "Resources.h"

#include <Windows.h>

// ReSharper disable once StringLiteralTypo
std::wstring Framework::Resources::m_GlobalPath = LR"(D:\VirtualWorld\resources\)";
std::wstring Framework::Resources::m_LocalPath;

void Framework::Resources::Init()
{
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(nullptr, buffer, MAX_PATH);

	int nrArgs;
	LPWSTR* args = CommandLineToArgvW(GetCommandLineW(), &nrArgs);

	if (nrArgs >= 2)
		m_LocalPath = args[1];
	else
	{
		Logger::PrintError("Local resources path should be defined in args");
		return;
	}

	LocalFree(args);
}

std::wstring Framework::Resources::Global(const std::wstring& subPath)
{
	return std::wstring(m_GlobalPath + subPath);
}

std::wstring Framework::Resources::GlobalShader(const std::wstring& subPath)
{
	return std::wstring(m_GlobalPath + L"Shaders\\" + subPath);
}

std::wstring Framework::Resources::Local(const std::wstring& subPath)
{
	return std::wstring(m_LocalPath + subPath);
}

std::wstring Framework::Resources::LocalShader(const std::wstring& subPath)
{
	return std::wstring(m_LocalPath + L"Shaders\\" + subPath);
}
