#pragma once
#include "Math/Rects.h"

namespace MyEngine
{
	namespace Rendering
	{
		namespace Font
		{
			class FontAtlas;
		}

		class FontAtlasLookup
		{
		public:
			FontAtlasLookup() = default;
			explicit FontAtlasLookup(Font::FontAtlas&& fontAtlas);

			void Lookup(char c, float xPixelHeight, RectFloat& uvRect, Float2& screenSize);

			float GetCharUvWidth(char c);
			float GetCharUvHeight(char c);
			Float2 GetCharUvSize(char c);
			RectFloat GetCharUvRect(char c);

		private:
			Array<float> m_Positions{};
			Array<float> m_Heights{};
			float m_SpaceWidth{};

			float m_ToXRatio{}; //inverse x-uv-height. (multiply with this gives ratio to x)
			float m_UvWidthToHeight{}; //convert x-uv in same unit as y-uv (

			static int CharToIdx(char c);
		};
	}
}
