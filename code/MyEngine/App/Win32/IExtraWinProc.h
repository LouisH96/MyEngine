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

				virtual void OnResize(Math::Int2 newSize) {}
				virtual void OnPaint() {}
				virtual void OnKeyUp(char c) {}
				virtual void OnKeyDown(char c) {}
			};
		}
	}
}
