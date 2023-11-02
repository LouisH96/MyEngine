#pragma once
#include "Geometry/Shapes/Rects.h"

namespace MyEngine
{
	namespace Rendering
	{
		namespace Font
		{
			class FontAtlas;
		}
	}

	//scale 1 = the height of the x-character
	class TextAssembler
	{
	public:
		TextAssembler() = default;
		explicit TextAssembler(Rendering::Font::FontAtlas&& fontAtlas);
		~TextAssembler();

		TextAssembler(const TextAssembler& other) = delete;
		TextAssembler(TextAssembler&& other) noexcept;
		TextAssembler& operator=(const TextAssembler& other) = delete;
		TextAssembler& operator=(TextAssembler&& other) noexcept;

		float GetMaxTextHeight(float scale) const;
		Float2 GetSize(const std::string& text, float scale, float& baseline) const;
		Float2 GetSize(const std::string& text, const Float2& scale, float& baseline) const;

		//new pivot! [0-1]
		template<typename Combinator, typename Adder>
		RectFloat AssembleInto(const Combinator& combinator, Adder&& adder, const Float2& position, const Float2& pivot, const std::string& text, const Float2& scale);

		template<typename Combinator, typename Adder>
		RectFloat AssembleInto(const Combinator& combinator, Adder&& adder, const Float2& position, const Float2& pivot, const std::string& text, const Float2& scale, const Float2& textSize, float baseline);

	private:
		static constexpr unsigned DATA_POSITIONS_IDX{ 0 };
		static constexpr unsigned DATA_HEIGHTS_IDX{ 1 };
		static constexpr unsigned DATA_VER_OFFSET_IDX{ 2 };
		static constexpr unsigned DATA_NR_PROPS{ 3 };
		unsigned m_NrCharacters{};
		float* m_pData{}; //first xPositions, than heights, than verOffset

		float m_MaxTextHeight{}; //the max height of a text (tail & head included), in x-space
		float m_SpaceWidthInWuvSpace; //in width-uv-space
		float m_HuvSpaceToXSpace{}; // (1 / UvHeightOfX) -> convert from height-uv-space to x-space
		float m_WuvSpaceToXSpace{}; // convert width-uv-space to x-space

		float GetHeightInHuvSpace(unsigned charIdx) const; //height-uv-space
		float GetVerOffsetInHuvSpace(unsigned charIdx) const;
		float GetVerOffsetInXSpace(unsigned charIdx) const;
		float GetWidthInWuvSpace(unsigned charIdx) const;
		float GetWidthInXSpace(unsigned charIdx) const;

		Float2 GetSize(const std::string& text, float& baseline) const; //in x-space
		RectFloat GetUvRect(unsigned charIdx) const;

		float* GetCharData(unsigned charIdx) const;
		static unsigned CharToIdx(char character);
	};

	template <typename Combinator, typename Adder>
	RectFloat TextAssembler::AssembleInto(const Combinator& combinator, Adder&& adder,
		const Float2& position, const Float2& pivot, const std::string& text, const Float2& scale)
	{
		float baseline;
		const Float2& textSize{ GetSize(text, scale, baseline) };
		return AssembleInto(combinator, adder, position, pivot, text, scale, textSize, baseline);
	}

	template <typename Combinator, typename Adder>
	RectFloat TextAssembler::AssembleInto(const Combinator& combinator, Adder&& adder, const Float2& position,
		const Float2& pivot, const std::string& text, const Float2& scale, const Float2& textSize, float baseline)
	{
		RectFloat bounds{
			{},
			textSize
		};
		bounds.SetLeftBot({ position - pivot * bounds.GetSize() });
		baseline += bounds.GetBottom();

		RectFloat posRect{};
		posRect.SetLeft(bounds.GetLeft());

		for (const char character : text)
		{
			if (character == ' ')
			{
				posRect.SetLeft(posRect.GetLeft() + m_SpaceWidthInWuvSpace * m_WuvSpaceToXSpace * scale.x);
				continue;
			}
			const unsigned charIdx{ CharToIdx(character) };

			const RectFloat uvRect{ GetUvRect(charIdx) };

			posRect.SetSize({
				uvRect.GetWidth() * m_WuvSpaceToXSpace * scale.x,
				uvRect.GetHeight() * m_HuvSpaceToXSpace * scale.y
				});
			posRect.SetBottom(baseline + GetVerOffsetInXSpace(charIdx) * scale.y);

			adder.Add(
				combinator(posRect.GetLeftBot(), uvRect.GetLeftBot()),
				combinator(posRect.GetLeftTop(), { uvRect.GetLeft(),0 }),
				combinator(posRect.GetRightBot(), uvRect.GetRightBot())
			);
			adder.Add(
				combinator(posRect.GetLeftTop(), { uvRect.GetLeft(),0 }),
				combinator(posRect.GetRightTop(), { uvRect.GetRight(),0 }),
				combinator(posRect.GetRightBot(), uvRect.GetRightBot())
			);

			posRect.SetLeft(posRect.GetLeft() + posRect.GetWidth());
		}
		return bounds;
	}
}
