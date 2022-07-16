#pragma once

#define SAFE_RELEASE(x) if(x) { (x)->Release(); (x) = nullptr; }

namespace MyEngine
{
	namespace Rendering
	{
		class DxHelper
		{

		};
	}
}
