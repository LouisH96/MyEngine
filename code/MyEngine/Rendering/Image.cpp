#include "pch.h"
#include "Image.h"

Rendering::Image::Image(int width, int height)
	: m_Width{ width }
	, m_Height{ height }
	, m_pData{ new uint8_t[width * height * 4] }
{
}

Rendering::Image::~Image()
{
	delete[] m_pData;
}

void Rendering::Image::Clear()
{
	m_pData = nullptr;
}

bool Rendering::Image::IsCleared() const
{
	return m_pData == nullptr;
}

void Rendering::Image::SetColor(int col, int row, uint8_t r, uint8_t g, uint8_t b, uint8_t a) const
{
	uint8_t* pColor{ &m_pData[(row * m_Width + col) * 4] };
	pColor[0] = r;
	pColor[1] = g;
	pColor[2] = b;
	pColor[3] = a;
}

void Rendering::Image::SetColor(int col, int row, const Math::Float3& color, float a) const
{
	uint8_t* pColor{ &m_pData[(row * m_Width + col) * 4] };
	pColor[0] = static_cast<uint8_t>(color.x * 255.f);
	pColor[1] = static_cast<uint8_t>(color.y * 255.f);
	pColor[2] = static_cast<uint8_t>(color.z * 255.f);
	pColor[3] = static_cast<uint8_t>(a * 255.f);
}

void Rendering::Image::SetRowColor(int row, const Math::Float3& color, float a) const
{
	const uint8_t r8{ static_cast<uint8_t>(color.x * 255.f) };
	const uint8_t g8{ static_cast<uint8_t>(color.y * 255.f) };
	const uint8_t b8{ static_cast<uint8_t>(color.z * 255.f) };
	const uint8_t a8{ static_cast<uint8_t>(a * 255.f) };
	uint8_t* pChannel{ &m_pData[row * m_Width * 4] };
	for(int i = 0; i < m_Width; i++)
	{
		*pChannel++ = r8;
		*pChannel++ = g8;
		*pChannel++ = b8;
		*pChannel++ = a8;
	}
}
