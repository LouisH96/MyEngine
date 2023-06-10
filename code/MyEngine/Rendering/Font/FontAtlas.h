#pragma once

namespace MyEngine
{
	namespace Io
	{
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
				FontAtlas();
				~FontAtlas();

				Image& GetImage() { return *m_pImage; }
				Image* GetImageOwnership();

				const Float2& GetWorldSize() const { return m_WorldSize; }

			private:
				Image* m_pImage;
				Float2 m_WorldSize; //temp

				static Float2 GetBounds(const Io::TtfReader& reader, float ttfToPixels);
				static void DoBoundsStep(const Io::TtfReader& reader, Int2& bounds, float ttfToPixels, char character);

				static void DrawAtlasStep(const Io::TtfReader& reader, Image& atlasImage, int& x, float ttfToPixels, char character);
			};
		}
	}
}
