#include "pch.h"
#include "RawInput.h"

#include <hidusage.h>

namespace MyEngine::App::Win32
{
// Expected MouseMessageSize: 48
// FastMouse/NormalFps: 1000/60: 17 -*48-> 800
// FastMouse/FastFps: 1000/120: 8 -*48-> 400
// NormalMouse/NormalFps: 125/60: 2 -*48-> 100
// NormalMouse/FastFps: 125/120: 1 -*48-> 50
RawInput::RawInput()
	: m_BufferSize{ 512 } //if not big enough -> will dynamically double
	, m_pBuffer{}
{
}

void RawInput::Init(HWND windowHandle)
{
	RegisterMouse(windowHandle);
	CreateBuffer();
}

RawInput::~RawInput()
{
	ReleaseBuffer();
	m_pBuffer = nullptr;
}

void RawInput::Process(Mouse& mouse)
{
	const unsigned received1{ GetAndProcess(mouse) };
	if (received1 == 0)
		return;

	//Get it a second time to see if everything in the queue was handled;
	const unsigned received2{ GetAndProcess(mouse) };
	if (received2 == 0)
		return;

	//Second call received bytes:
	//	This could be because the buffer was not big enough,
	//	or because a new message arrived after the first call.
	if ((received1 + received2) > m_BufferSize
		&& Globals::Time > .1f)
	{
		m_BufferSize *= 2;
		ReleaseBuffer();
		CreateBuffer();
	}

	//Process entire queue with the current grown buffer.
	//	No more growing to take exceptional long frames into account.
	for (unsigned i{ 0 }; i < 1000; ++i)
		if (!GetAndProcess(mouse))
			return;
	Logger::Warning("[RawInput::Process] Not all input processed after a 1000 buffers");
}

void RawInput::RegisterMouse(HWND windowHandle)
{
	RAWINPUTDEVICE rawMouse{};
	rawMouse.usUsagePage = HID_USAGE_PAGE_GENERIC;
	rawMouse.usUsage = HID_USAGE_GENERIC_MOUSE;
	rawMouse.dwFlags = 0;
	rawMouse.hwndTarget = windowHandle;

	if (!RegisterRawInputDevices(&rawMouse, 1, sizeof(rawMouse)))
		Logger::Error("[RawInput::RegisterMouse] Failed registering raw mouse");
}

void RawInput::CreateBuffer()
{
	m_pBuffer = new Message[m_BufferSize];
}

void RawInput::ReleaseBuffer()
{
	delete[] m_pBuffer;
}

unsigned RawInput::GetAndProcess(Mouse& mouse)
{
	unsigned size{ m_BufferSize };
	const unsigned count{ GetRawInputBuffer(m_pBuffer, &size, sizeof(RAWINPUTHEADER)) };

	if (count == static_cast<UINT>(-1))
		Logger::Error("[RawInput::GetAndProcess] Failed getting RawInputBuffer");

	Message* pMessage{ m_pBuffer };
	for (unsigned i{ 0 }; i < count; ++i)
	{
		if (pMessage->header.dwType == RIM_TYPEMOUSE)
		{
			mouse.m_Movement += Int2{
				pMessage->data.mouse.lLastX,
				pMessage->data.mouse.lLastY };
		}

		using QWORD = uint64_t;
		pMessage = NEXTRAWINPUTBLOCK(pMessage);
	}

	return static_cast<unsigned>(
		reinterpret_cast<byte*>(pMessage) -
		reinterpret_cast<byte*>(m_pBuffer));
}
}
