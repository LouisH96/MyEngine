#include "pch.h"
#include "Resources.h"

#include <Windows.h>

// ReSharper disable once StringLiteralTypo
std::wstring Resources::m_GlobalPath;
std::wstring Resources::m_LocalPath{ LR"(Resources\)" };

void Resources::Init()
{
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(nullptr, buffer, MAX_PATH);

	int nrArgs;
	LPWSTR* args = CommandLineToArgvW(GetCommandLineW(), &nrArgs);

	if (nrArgs >= 2)
		m_GlobalPath = args[1];
	else
	{
		Logger::PrintError("Global resources path should be defined in args");
		return;
	}

	LocalFree(args);
}

std::wstring Resources::Global(const std::wstring& subPath)
{
	return std::wstring(m_GlobalPath + subPath);
}

std::wstring Resources::GlobalShader(const std::wstring& subPath)
{
	return std::wstring(m_GlobalPath + L"Shaders\\" + subPath);
}

std::wstring Resources::Local(const std::wstring& subPath)
{
	return std::wstring(m_LocalPath + subPath);
}

std::wstring Resources::LocalShader(const std::wstring& subPath)
{
	return std::wstring(m_LocalPath + L"Shaders\\" + subPath);
}
