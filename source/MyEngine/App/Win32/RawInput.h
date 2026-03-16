#pragma once
#include "Mouse.h"
#include <WinUser.h>

namespace MyEngine::App::Win32
{
class RawInput
{
public:
	RawInput();
	void Init(HWND windowHandle);
	~RawInput();

	void Process(Mouse& mouse);

private:
	using Message = RAWINPUT;

	Message* m_pBuffer;
	unsigned m_BufferSize;

	void CreateBuffer();
	void ReleaseBuffer();
	void RegisterMouse(HWND windowHandle);

	unsigned GetAndProcess(Mouse& mouse);
};
}