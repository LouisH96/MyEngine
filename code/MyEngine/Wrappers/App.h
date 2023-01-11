#pragma once
#include "Framework.h"

namespace MyEngine
{
	namespace Wrappers
	{
	class App
	{
	public:
		void Run();

	private:
		Framework m_Framework{};
	};
	}
}

