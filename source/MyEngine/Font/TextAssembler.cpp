#include "pch.h"
#include "TextAssembler.h"

#include "Rendering/Font/FontAtlas.h"

TextAssembler::TextAssembler(Rendering::Font::FontAtlas&& fontAtlas)
	: m_NrCharacters{ fontAtlas.GetCharacterHeight().GetSize() }
{
	m_pData = new float[m_NrCharacters * DATA_NR_PROPS + 1];

	//---| Set data properties |---
	float* pData = m_pData;
	const float* sourceXPos{ fontAtlas.GetCharacterHorPos().GetData() };
	const float* sourceHeights{ fontAtlas.GetCharacterHeight().GetData() };
	const float* sourceBaselineOffset{ fontAtlas.GetBaselineOffset().GetData() };
	const float* sourceHorOffset{ fontAtlas.GetHorOffset().GetData() };

	float lowest{ Float::MAX };
	float highest{ -Float::MAX };

	for (unsigned i = 0; i < m_NrCharacters; i++, pData += DATA_NR_PROPS)
	{
		float& xPos{ pData[DATA_POSITIONS_IDX] };
		float& height{ pData[DATA_HEIGHTS_IDX] };
		float& verOffset{ pData[DATA_VER_OFFSET_IDX] };
		float& horOffset{ pData[DATA_HOR_OFFSET_IDX] };

		xPos = *sourceXPos++;
		height = *sourceHeights++;
		verOffset = *sourceBaselineOffset++;
		horOffset = *sourceHorOffset++;

		const float charLowest{ verOffset };
		const float charHighest{ charLowest + height };
		if (charLowest < lowest) lowest = charLowest;
		if (charHighest > highest) highest = charHighest;
	}
	*pData = *sourceXPos;

	//---| Set other values |---
	m_HuvSpaceToXSpace = 1.f / GetHeightInHuvSpace(CharToIdx('x'));
	m_MaxTextHeight = (highest - lowest) * m_HuvSpaceToXSpace;
	m_WuvSpaceToXSpace = fontAtlas.GetImageAspectRatio() * m_HuvSpaceToXSpace;
	m_SpaceWidthInWuvSpace = GetWidthInWuvSpace(CharToIdx(' '));
}

TextAssembler::~TextAssembler()
{
	delete[] m_pData;
}

TextAssembler::TextAssembler(TextAssembler&& other) noexcept
	: m_NrCharacters{ other.m_NrCharacters }
	, m_pData{ other.m_pData }
	, m_MaxTextHeight{ other.m_MaxTextHeight }
	, m_SpaceWidthInWuvSpace{ other.m_SpaceWidthInWuvSpace }
	, m_HuvSpaceToXSpace{ other.m_HuvSpaceToXSpace }
	, m_WuvSpaceToXSpace{ other.m_WuvSpaceToXSpace }
{
	other.m_NrCharacters = 0;
	other.m_pData = nullptr;
}

TextAssembler& TextAssembler::operator=(TextAssembler&& other) noexcept
{
	if (&other == this) return *this;

	delete[] m_pData;
	m_pData = other.m_pData;
	other.m_pData = nullptr;

	m_NrCharacters = other.m_NrCharacters;
	other.m_NrCharacters = 0;

	m_MaxTextHeight = other.m_MaxTextHeight;
	m_SpaceWidthInWuvSpace = other.m_SpaceWidthInWuvSpace;
	m_HuvSpaceToXSpace = other.m_HuvSpaceToXSpace;
	m_WuvSpaceToXSpace = other.m_WuvSpaceToXSpace;

	return *this;
}

float TextAssembler::GetMaxTextHeight(float scale) const
{
	return m_MaxTextHeight * scale;
}

Float2 TextAssembler::GetSize(const std::string& text, float scale, float& baseline) const
{
	const Float2 size{ GetSize(text, baseline) * scale };
	baseline *= scale;
	return size;
}

Float2 TextAssembler::GetSize(const std::string& text, const Float2& scale, float& baseline) const
{
	const Float2 size{ GetSize(text, baseline) * scale };
	baseline *= scale.y;
	return size;
}

Float2 TextAssembler::GetSize_XCenter(const std::string& text, float scale, float& baseline) const
{
	return GetSize_XCenter(text, Float2{ scale, scale }, baseline);
}

Float2 TextAssembler::GetSize_XCenter(const std::string& text, const Float2& scale, float& baseline) const
{
	Float2 size{ GetSize(text, scale, baseline) };

	const float xTop{ (GetCharData('x')[DATA_VER_OFFSET_IDX] + GetCharData('x')[DATA_HEIGHTS_IDX]) * scale.y * m_HuvSpaceToXSpace + baseline };

	const float below{ baseline };
	const float above{ size.y - xTop };

	if (below > above)
		size.y += below - above;
	/*else
	{
		const float higher{ above - below };
		baseline += higher;
		size.y += higher;
	}*/
	//note: not increasing bottom seems to look better

	return size;
}

Float2 TextAssembler::GetSize(const std::string& text, float& baseline) const
{
	//baseline = lowest
	baseline = Float::MAX;
	Float2 size{ 0, -Float::MAX }; //y = highest

	for (unsigned iChar{ 0 }; iChar < text.size(); ++iChar)
	{
		const char& character{ text[iChar] };
		if (character == ' ')
		{
			size.x += m_SpaceWidthInWuvSpace;
			continue;
		}
		const unsigned charIdx{ CharToIdx(character) };
		const float* pData{ GetCharData(charIdx) };

		const float charBottom{ pData[DATA_VER_OFFSET_IDX] };
		const float charTop{ charBottom + pData[DATA_HEIGHTS_IDX] };
		if (charBottom < baseline) baseline = charBottom;
		if (charTop > size.y) size.y = charTop;

		pData += DATA_POSITIONS_IDX;
		size.x += -*pData + *(pData + DATA_NR_PROPS); // next char position - current position

		if (iChar != 0)
			size.x += pData[DATA_HOR_OFFSET_IDX]; //horizontal padding before character
	}

	size.y -= baseline;
	baseline = -baseline * m_HuvSpaceToXSpace;
	size.y *= m_HuvSpaceToXSpace;
	size.x *= m_WuvSpaceToXSpace;
	return size;
}

RectFloat TextAssembler::GetUvRect(unsigned charIdx) const
{
	const float* pData{ GetCharData(charIdx) };

	return RectFloat{
		{pData[DATA_POSITIONS_IDX], pData[DATA_HEIGHTS_IDX]},
		{pData[DATA_NR_PROPS + DATA_POSITIONS_IDX] - pData[DATA_POSITIONS_IDX], pData[DATA_HEIGHTS_IDX]}
	};
}

float* TextAssembler::GetCharData(unsigned charIdx) const
{
	return &m_pData[static_cast<size_t>(DATA_NR_PROPS) * charIdx];
}

float* TextAssembler::GetCharData(char character) const
{
	return GetCharData(CharToIdx(character));
}

float TextAssembler::GetHeightInHuvSpace(unsigned charIdx) const
{
	return m_pData[static_cast<size_t>(DATA_NR_PROPS) * charIdx + DATA_HEIGHTS_IDX];
}

float TextAssembler::GetVerOffsetInHuvSpace(unsigned charIdx) const
{
	return m_pData[static_cast<size_t>(DATA_NR_PROPS) * charIdx + DATA_VER_OFFSET_IDX];
}

float TextAssembler::GetVerOffsetInXSpace(unsigned charIdx) const
{
	return GetVerOffsetInHuvSpace(charIdx) * m_HuvSpaceToXSpace;
}

float TextAssembler::GetHorOffsetInXSpace(unsigned charIdx) const
{
	return m_pData[static_cast<size_t>(DATA_NR_PROPS) * charIdx + DATA_HOR_OFFSET_IDX] * m_WuvSpaceToXSpace;
}

float TextAssembler::GetWidthInWuvSpace(unsigned charIdx) const
{
	const float* pData{ GetCharData(charIdx) + DATA_POSITIONS_IDX };
	return pData[DATA_NR_PROPS] - *pData;
}

float TextAssembler::GetWidthInXSpace(unsigned charIdx) const
{
	return GetWidthInWuvSpace(charIdx) * m_WuvSpaceToXSpace;
}

unsigned TextAssembler::CharToIdx(char character)
{
	return static_cast<unsigned>(character);
}
