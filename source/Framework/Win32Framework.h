#pragma once
#include <string>

namespace MyEngine
{
	namespace Framework
	{
		class Win32Framework
		{
		public:
			Win32Framework(const std::wstring& title);

		private:
			static void Init(const std::wstring& title);
		};
	}
}
