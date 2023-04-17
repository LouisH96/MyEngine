#pragma once
#include "Framework/BasicFramework.h"

namespace MyEngine
{
	namespace App
	{
		class BasicExampleApp
		{
		public:
			void Run();

		private:
			Framework::BasicFramework m_Framework{};
		};
	}
}
