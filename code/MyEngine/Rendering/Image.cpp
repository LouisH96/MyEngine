#include "pch.h"
#include "Image.h"

#include "Math/ScalarMath.h"

Rendering::Image::Image(int width, int height)
	: m_Width{ width }
	, m_Height{ height }
	, m_pData{ new uint8_t[width * height * 4] }
{
}

Rendering::Image::Image(const Float2& size)
	: Image{ FloatMath::Ceil(size.x), FloatMath::Ceil(size.y) }
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

void Rendering::Image::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	for (int i = 0; i < m_Height; i++)
		SetRowColor(i, r, g, b, a);
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

void Rendering::Image::SetRowColor(int row, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	uint8_t* pChannel{ &m_pData[row * m_Width * 4] };
	for (int i = 0; i < m_Width; i++)
	{
		*pChannel++ = r;
		*pChannel++ = g;
		*pChannel++ = b;
		*pChannel++ = a;
	}
}

void Rendering::Image::SetRowColor(int row, const Math::Float3& color, float a)
{
	const uint8_t r{ static_cast<uint8_t>(color.x * 255.f) };
	const uint8_t g{ static_cast<uint8_t>(color.y * 255.f) };
	const uint8_t b{ static_cast<uint8_t>(color.z * 255.f) };
	const uint8_t a8{ static_cast<uint8_t>(a * 255.f) };
	SetRowColor(row, r, g, b, a8);
}

uint8_t* Rendering::Image::GetPixel(const Int2& pos)
{
	return m_pData + (pos.y * m_Width + pos.x) * 4;
}

void Rendering::Image::CopyTo(Image& dest, const Int2& position) const
{
	if (!dest.IsInBounds(position + GetSize() - Int2{1, 1}))
	{
		Logger::PrintError("Image would be copied outside destination image");
		//return;
	}

	const int destStep = dest.GetWidth() * 4;
	const int sourceStep = GetWidth() * 4;

	uint8_t* pDest{ dest.GetPixel(position) };
	const uint8_t* pSource{ m_pData };

	for (int iRow = 0; iRow < GetHeight(); iRow++)
	{
		memcpy(pDest, pSource, sourceStep);
		pDest += destStep;
		pSource += sourceStep;
	}
}

bool Rendering::Image::IsInBounds(const Int2& pos) const
{
	return pos.x >= 0 && pos.x < m_Width
		&& pos.y >= 0 && pos.y < m_Height;
}
