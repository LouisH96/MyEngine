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
