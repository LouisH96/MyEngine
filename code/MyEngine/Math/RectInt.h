#pragma once

namespace MyEngine
{
	namespace Math
	{
		class RectInt
		{
		private:
			int m_X;
			int m_Y;
			int m_Width;
			int m_Height;

		public:
			RectInt();
			RectInt(int x, int y, int width, int height);

			int GetX() const { return m_X; }
			int GetY() const { return m_Y; }
			int GetWidth() const { return m_Width; }
			int GetHeight() const { return m_Height; }

			int CenterX() const { return m_X + m_Width / 2; }
			int CenterY() const { return m_Y + m_Height / 2; }
		};
	}
}
