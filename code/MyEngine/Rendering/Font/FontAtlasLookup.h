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

			float GetSpaceWidth() const { return m_SpaceWidthRatio; }
			float GetCharUvWidth(char c);
			float GetCharUvHeight(char c);
			Float2 GetCharUvSize(char c);
			RectFloat GetCharUvRect(char c);

			float GetUvWidth(const std::string& text);
			float GetScreenWidth(const std::string& text, float height);
			float GetScreenWidth(char c, float height);
			float GetScreenSpaceWidth(float xHeight);

		private:
			Array<float> m_Positions{};
			Array<float> m_Heights{};
			float m_SpaceWidthRatio{};

			float m_InvXUvHeight{};
			float m_UvWidthToHeight{}; //convert x-uv in same unit as y-uv (

			static int CharToIdx(char c);
		};
	}
}
