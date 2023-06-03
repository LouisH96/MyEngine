#pragma once
#include "Math/Vector2.h"

namespace MyEngine
{
	namespace Math
	{
		template<typename T>
		class Rect
		{
		public:
			Rect() = default;
			Rect(const Vector2<T>& leftBot, const Vector2<T>& size);

			const Vector2<T>& GetLeftBot() const { return m_LeftBot; }
			const Vector2<T>& GetSize() const { return m_Size; }
			Vector2<T> GetLeftTop() const;
			Vector2<T> GetRightBot() const;
			Vector2<T> GetRightTop() const;

			const T& GetWidth() const { return m_Size.x; }
			const T& GetHeight() const { return m_Size.y; }

			T GetLeft() const;
			T GetRight() const;
			T GetBottom() const;
			T GetTop() const;

			void SetLeftBot(const Vector2<T>& leftBot) { m_LeftBot = leftBot; }
			void SetSize(const Vector2<T>& size) { m_Size = size; }
			void SetLeft(const T& left) { m_LeftBot.x = left; }
			void SetBottom(const T& bottom) { m_LeftBot.y = bottom; }
			void SetWidth(const T& width) { m_Size.x = width; }
			void SetHeight(const T& height) { m_Size.y = height; }

		private:
			Vector2<T> m_LeftBot;
			Vector2<T> m_Size;
		};

		template <typename T>Rect<T>::Rect(const Vector2<T>& leftBot, const Vector2<T>& size) : m_LeftBot{ leftBot }, m_Size{ size } {}

		template <typename T>Vector2<T> Rect<T>::GetLeftTop() const { return { m_LeftBot.x, m_LeftBot.y + m_Size.y }; }
		template <typename T>Vector2<T> Rect<T>::GetRightBot() const { return { m_LeftBot.x + m_Size.x, m_LeftBot.y }; }
		template <typename T>Vector2<T> Rect<T>::GetRightTop() const { return m_LeftBot + m_Size; }

		template <typename T>T Rect<T>::GetLeft() const { return m_LeftBot.x; }
		template <typename T>T Rect<T>::GetRight() const { return m_LeftBot.x + m_Size.x; }
		template <typename T>T Rect<T>::GetBottom() const { return m_LeftBot.y; }
		template <typename T>T Rect<T>::GetTop() const { return m_LeftBot.y + m_Size.y; }
	}
}
