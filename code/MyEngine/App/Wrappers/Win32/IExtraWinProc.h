#pragma once
#include "Window.h"

namespace DirectX
{
	struct XMINT2;
}

namespace MyEngine
{
	namespace App
	{
		namespace Wrappers
		{
			namespace Win32
			{
				class IExtraWinProc
				{
				public:
					IExtraWinProc(const IExtraWinProc& other) = delete;
					IExtraWinProc(IExtraWinProc&& other) noexcept = delete;
					IExtraWinProc& operator=(const IExtraWinProc& other) = delete;
					IExtraWinProc& operator=(IExtraWinProc&& other) noexcept = delete;

					IExtraWinProc() = default;
					virtual ~IExtraWinProc() = default;
					
					virtual void OnResize(const DirectX::XMINT2& newSize) = 0;
				};
			}
		}
	}
}
