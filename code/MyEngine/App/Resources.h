#pragma once
#include <string>

namespace MyEngine
{
	namespace App
	{
		class Resources
		{
		public:
			static void Init();
			static std::wstring GetResourcePath(const std::wstring& subPath);
			static std::wstring GetShaderPath(const std::wstring& subPath);

		private:
			static std::wstring m_ResourceFolderPath;
		};
	}
}
