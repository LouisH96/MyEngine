#pragma once

#define SAFE_RELEASE(x) if(x) { (x)->Release(); (x) = nullptr; }

namespace MyEngine
{
	namespace DirectX
	{
		class DxHelper
		{

		};
	}
}
