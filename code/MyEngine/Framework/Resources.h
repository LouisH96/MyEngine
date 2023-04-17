#pragma once
#include <string>

namespace MyEngine
{
	namespace Framework
	{
		class Resources
		{
		public:
			static void Init();
			static std::wstring GetGlobalResourcePath(const std::wstring& subPath);
			static std::wstring GetGlobalShaderPath(const std::wstring& subPath);
			static std::wstring GetLocalResourcePath(const std::wstring& subPath);
			static std::wstring GetLocalShaderPath(const std::wstring& subPath);

		private:
			static std::wstring m_GlobalResourcesPath;
			static std::wstring m_LocalResourcesPath;
		};
	}
}
