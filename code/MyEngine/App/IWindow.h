#pragma once

#include "DirectXMath.h"

namespace MyEngine
{
	namespace App
	{
		class IWindowResizeListener;

		class IWindow
		{
		public:
			IWindow(const IWindow& other) = delete;
			IWindow(IWindow&& other) noexcept = delete;
			IWindow& operator=(const IWindow& other) = delete;
			IWindow& operator=(IWindow&& other) noexcept = delete;

			IWindow() = default;
			virtual ~IWindow() = default;

			virtual void Init(const std::wstring& title) = 0;
			virtual void Release() = 0;

			virtual void DispatchEvents() = 0;

			virtual void Listen(IWindowResizeListener& listener) = 0;

			virtual bool IsDestroyed() const = 0;
			virtual DirectX::XMINT2 GetSize() const = 0;
		};
	}
}
