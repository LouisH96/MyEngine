#pragma once
#include "Framework/CppFramework.h"

namespace MyEngine
{
	namespace App
	{
		class BasicExampleApp
		{
		public:
			void Run();

		private:
			Framework::CppFramework m_Framework{};
		};
	}
}
