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
			static std::wstring Global(const std::wstring& subPath);
			static std::wstring GlobalShader(const std::wstring& subPath);
			static std::wstring Local(const std::wstring& subPath);
			static std::wstring LocalShader(const std::wstring& subPath);

		private:
			static std::wstring m_GlobalPath;
			static std::wstring m_LocalPath;
		};
	}
}
