#pragma once

namespace MyEngine
{
	namespace Rendering
	{
		class Image
		{
		public:
			Image(int width, int height);
			explicit Image(const Float2& size);
			~Image();

			void Clear();
			bool IsCleared() const;

			void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
			void SetColor(int col, int row, uint8_t r, uint8_t g, uint8_t b, uint8_t a) const;
			void SetColor(int col, int row, const Math::Float3& color, float a = 1.f) const;
			void SetRowColor(int row, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
			void SetRowColor(int row, const Math::Float3& color, float a = 1.f);

			int GetWidth() const { return m_Width; }
			int GetHeight() const { return m_Height; }
			Int2 GetSize() const { return { m_Width, m_Height }; }
			uint8_t* GetData() const { return m_pData; }
			uint8_t* GetPixel(const Int2& pos);
			int GetBytesPerRow() const { return m_Width * 4; }

			void CopyTo(Image& dest, const Int2& position) const;
			bool IsInBounds(const Int2& pos) const;

		private:
			const int m_Width;
			const int m_Height;
			uint8_t* m_pData;
		};
	}
}
