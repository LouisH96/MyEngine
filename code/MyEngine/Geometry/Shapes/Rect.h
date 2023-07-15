#pragma once
#include "Math/Vector2.h"

#undef min
#undef max

namespace MyEngine
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
		T GetCenter() const;

		T GetTopInv() const; //bottom - height
		T GetRightInv() const; //left - width

		void SetLeftBot(const Vector2<T>& leftBot) { m_LeftBot = leftBot; }
		void SetSize(const Vector2<T>& size) { m_Size = size; }
		void SetLeft(const T& left) { m_LeftBot.x = left; }
		void SetBottom(const T& bottom) { m_LeftBot.y = bottom; }
		void SetWidth(const T& width) { m_Size.x = width; }
		void SetHeight(const T& height) { m_Size.y = height; }

		void SetRightUsePos(const T& right);
		void SetTopUsePos(const T& top);

		void Expand(const Vector4<T>& expand); //left, up, right, bot
		Rect Expanded(const Vector4<T>& expand) const; //left, up, right, bot
		void Compress(const Vector4<T>& compress); //left, up, right, bot
		Rect Compressed(const Vector4<T>& compress) const; //left, up, right, bot
		void Move(const Vector2<T>& movement);
		Rect Moved(const Vector2<T>& movement) const;

		Vector2<T> Clamped(const Vector2<T>& vector) const;
		void Clamp(Vector2<T>& vector) const;

		Rect ClampedPos(const Rect& rect) const;
		void ClampPos(Rect& rect) const;

		bool IsFullyInside(const Rect& outer) const;

		Float2 GetPointRelative(const Float2& ratio) const;

		static Rect Bounds(const Vector2<T>* pData, unsigned count);

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

	template <typename T>
	T Rect<T>::GetCenter() const
	{
		return m_LeftBot + m_Size / 2;
	}

	template<typename T> T Rect<T>::GetTopInv() const { return m_LeftBot.y - m_Size.y; }
	template<typename T> T Rect<T>::GetRightInv() const { return m_LeftBot.x - m_Size.x; }

	template <typename T>
	void Rect<T>::SetRightUsePos(const T& right)
	{
		m_LeftBot.x = right - GetWidth();
	}

	template <typename T>
	void Rect<T>::SetTopUsePos(const T& top)
	{
		m_LeftBot.y = top - GetHeight();
	}

	template <typename T>
	void Rect<T>::Expand(const Vector4<T>& expand)
	{
		m_LeftBot.x -= expand.x;
		m_LeftBot.y -= expand.w;
		m_Size.x += expand.x + expand.z;
		m_Size.y += expand.y + expand.w;
	}

	template <typename T>
	Rect<T> Rect<T>::Expanded(const Vector4<T>& expand) const
	{
		return{
			{m_LeftBot.x - expand.x, m_LeftBot.y - expand.w},
			{m_Size.x + expand.x + expand.z, m_Size.y + expand.y + expand.w}
		};
	}

	template <typename T>
	void Rect<T>::Compress(const Vector4<T>& compress)
	{
		m_LeftBot.x += compress.x;
		m_LeftBot.y += compress.w;
		m_Size.x -= compress.x + compress.z;
		m_Size.y -= compress.y + compress.w;
	}

	template <typename T>
	Rect<T> Rect<T>::Compressed(const Vector4<T>& compress) const
	{
		return{
			{m_LeftBot.x + compress.x, m_LeftBot.y + compress.w},
			{m_Size.x - compress.x - compress.z, m_Size.y - compress.y - compress.w}
		};
	}

	template <typename T>
	void Rect<T>::Move(const Vector2<T>& movement)
	{
		m_LeftBot += movement;
	}

	template <typename T>
	Rect<T> Rect<T>::Moved(const Vector2<T>& movement) const
	{
		return{ m_LeftBot += movement, m_Size };
	}

	template <typename T>
	Vector2<T> Rect<T>::Clamped(const Vector2<T>& vector) const
	{
		return {
			Scalar<T>::Clamp(vector.x, m_LeftBot.x, m_LeftBot.x + m_Size.x - 1),
			Scalar<T>::Clamp(vector.y, m_LeftBot.y, m_LeftBot.y + m_Size.y - 1)
		};
	}

	template <typename T>
	void Rect<T>::Clamp(Vector2<T>& vector) const
	{
		vector.x = Scalar<T>::Clamp(vector.x, m_LeftBot.x, m_LeftBot.x + m_Size.x - 1);
		vector.y = Scalar<T>::Clamp(vector.y, m_LeftBot.y, m_LeftBot.y + m_Size.y - 1);
	}

	template <typename T>
	Rect<T> Rect<T>::ClampedPos(const Rect& rect) const
	{
		return { {
				Scalar<T>::Clamp(rect.m_LeftBot.x, m_LeftBot.x, m_LeftBot.x + m_Size.x - rect.m_Size.x - 1),
				Scalar<T>::Clamp(rect.m_LeftBot.y, m_LeftBot.y, m_LeftBot.y + m_Size.y - rect.m_Size.y - 1)
			}, m_Size };
	}

	template <typename T>
	void Rect<T>::ClampPos(Rect& rect) const
	{
		rect.m_LeftBot.x = Scalar<T>::Clamp(rect.m_LeftBot.x, m_LeftBot.x, m_LeftBot.x + m_Size.x - rect.m_Size.x - 1);
		rect.m_LeftBot.y = Scalar<T>::Clamp(rect.m_LeftBot.y, m_LeftBot.y, m_LeftBot.y + m_Size.y - rect.m_Size.y - 1);
	}

	template <typename T>
	bool Rect<T>::IsFullyInside(const Rect& outer) const
	{
		return m_LeftBot.x >= outer.m_LeftBot.x
			&& m_LeftBot.y >= outer.m_LeftBot.y
			&& GetRight() <= outer.GetRight()
			&& GetTop() <= outer.GetTop();
	}

	template <typename T>
	Float2 Rect<T>::GetPointRelative(const Float2& ratio) const
	{
		return m_LeftBot + m_Size * ratio;
	}

	template <typename T>
	Rect<T> Rect<T>::Bounds(const Vector2<T>* pData, unsigned count)
	{
		//store min in LeftBot & max in Size
		Rect bounds{ {std::numeric_limits<T>::max()}, {std::numeric_limits<T>::lowest()} };
		for (unsigned i = 0; i < count; i++)
		{
			const Vector2<T>& element{ pData[i] };
			if (element.x < bounds.m_LeftBot.x) bounds.m_LeftBot.x = element.x;
			if (element.y < bounds.m_LeftBot.y) bounds.m_LeftBot.y = element.y;
			if (element.x > bounds.m_Size.x) bounds.m_Size.x = element.x;
			if (element.y > bounds.m_Size.y) bounds.m_Size.y = element.y;
		}
		bounds.m_Size -= bounds.m_LeftBot;
		return bounds;
	}
}
