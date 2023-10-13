#pragma once
#include "Framework/Resources.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Ttf
		{
			class Glyph;
		}

		class TtfReader;
	}
	namespace Rendering
	{
		class Image;

		namespace Font
		{
			class FontAtlas
			{
			public:
				FontAtlas(int xHorizontalPixels, //amount of horizontal pixels in x-character
					const std::wstring& path = Resources::Global(LR"(Fonts\ShortBaby.ttf)"),
					const std::wstring& backupPath = Resources::Global(LR"(Fonts\Cascadia.ttf)"));
				~FontAtlas();

				Image& GetImage() { return *m_pImage; }
				Image* GetImageOwnership();

				Array<float>& GetCharacterHorPos() { return m_CharacterHorPos; }
				Array<float>& GetCharacterHeight() { return m_CharacterHeight; }
				float GetSpaceWidth() const { return m_SpaceWidth; }

			private:
				Image* m_pImage;
				Array<float> m_CharacterHorPos;
				Array<float> m_CharacterHeight;
				float m_SpaceWidth;

				void CharacterInfoStep(const Io::Ttf::Glyph& glyph, int idx, float ttfToPixels, float& highest);
				void DrawGlyphStep(const Io::Ttf::Glyph& glyph, int idx, float ttfToPixels);

				static void GetGlyph(int id,
					Io::TtfReader& mainReader, Io::TtfReader& backupReader, 
					float mainTtfToPixels, float backupTtfToPixels,
					Io::Ttf::Glyph& glyph, float& ttfToPixels);
			};
		}
	}
}
