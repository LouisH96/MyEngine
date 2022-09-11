#pragma once

#include "DirectXMath.h"

namespace MyEngine
{
	namespace App
	{
		namespace Input
		{
			class IInputWriter;
		}

		class IWindowResizeListener;

		class IWindow
		{
		public:
			//c++ object
			IWindow(const IWindow& other) = delete;
			IWindow(IWindow&& other) noexcept = delete;
			IWindow& operator=(const IWindow& other) = delete;
			IWindow& operator=(IWindow&& other) noexcept = delete;

			IWindow() = default;
			virtual ~IWindow() = default;

			//object
			virtual void Init(const std::wstring& title) = 0;
			virtual void Release() = 0;

			//messages
			virtual void HandleMessages() = 0;

			//screen
			virtual void Listen(IWindowResizeListener& listener) = 0;
			virtual bool IsDestroyed() const = 0;
			virtual DirectX::XMINT2 GetSize() const = 0;

			//input
			virtual void SetInputWriter(Input::IInputWriter& writer) = 0;
		};
	}
}
