#pragma once

namespace MyEngine
{
	template<typename T>
	class Rect3
	{
	public:
		Rect3();
		Rect3(const Vector3<T>& leftBot,
			const Vector3<T>& right, const Vector3<T>& up, const Vector3<T>& normal, const Vector2<T>& size);

		void SetPosition(const Vector3<T>& leftBot) { m_LeftBot = leftBot; }
		void SetSize(const Vector2<T>& size) { m_Size = size; }
		void SetWidth(T width) { m_Size.x = width; }
		void SetHeight(T height) { m_Size.y = height; }
		void SetRight(const Vector3<T>& right) { m_Right = right; } //should be normalized
		void SetUp(const Vector3<T>& up) { m_Up = up; } //should be normalized
		void SetNormal(const Vector3<T>& normal) { m_Normal = normal; } //should be normalized

		const Vector3<T>& GetLeftBot() const { return m_LeftBot; }
		const Vector3<T>& GetRight() const { return m_Right; } //should be normalized
		const Vector3<T>& GetUp() const { return m_Up; } //should be normalized
		const Vector3<T>& GetNormal() const { return m_Normal; } //should be normalized
		const Vector2<T>& GetSize() const { return m_Size; }
		float GetWidth() const { return m_Size.x; }
		float GetHeight() const { return m_Size.y; }

		Vector3<T>& GetLeftBot() { return m_LeftBot; }

		Vector3<T> GetLeftTop() const { return m_LeftBot + m_Up * m_Size.y; }
		Vector3<T> GetRightBot() const { return m_LeftBot + m_Right * m_Size.x; }
		Vector3<T> GetRightTop() const { return m_LeftBot + m_Right * m_Size.x + m_Up * m_Size.y; }

		Float3 GetPoint(const Float2& offset) const;
		Float3 GetPointRelative(const Float2& ratio) const;

		void MoveLocal(const Vector2<T>& translation, float alongNormal);
		Rect3 MovedLocal(const Vector3<T>& translation) const;

		Rect3 Compressed(const Vector2<T>& compression, T alongNormal = 0) const;
		Rect3 Expanded(const Vector2<T>& expansion, T alongNormal = 0) const;

		Rect3 MakeChild(const Vector2<T>& offset, const Vector2<T>& size) const;

		static Rect3 FromXyCenter(const Vector2<T>& center, const Vector2<T>& size, T depth = 0);

	private:
		Vector3<T> m_LeftBot;
		Vector3<T> m_Right;
		Vector3<T> m_Up;
		Vector3<T> m_Normal;
		Vector2<T> m_Size;
	};

	using Rect3Float = Rect3<float>;
	using Rect3Double = Rect3<double>;

	template <typename T>
	Rect3<T>::Rect3()
		: m_LeftBot{ 0 }
		, m_Right{ 1,0,0 }
		, m_Up{ 0,1,0 }
		, m_Normal{ 0,0,1 }
		, m_Size{ 1,1 }
	{ }

	template <typename T>
	Rect3<T>::Rect3(const Vector3<T>& leftBot, const Vector3<T>& right, const Vector3<T>& up,
		const Vector3<T>& normal, const Vector2<T>& size)
		: m_LeftBot{ leftBot }
		, m_Right{ right }
		, m_Up{ up }
		, m_Normal{ normal }
		, m_Size{ size }
	{
	}

	template <typename T>
	Float3 Rect3<T>::GetPoint(const Float2& offset) const
	{
		return m_LeftBot + m_Right * offset.x + m_Up * offset.y;
	}

	template <typename T>
	Float3 Rect3<T>::GetPointRelative(const Float2& ratio) const
	{
		return m_LeftBot + m_Right * (m_Size.x * ratio.x) + m_Up * (m_Size.y * ratio.y);
	}

	template <typename T>
	void Rect3<T>::MoveLocal(const Vector2<T>& translation, float alongNormal)
	{
		m_LeftBot += m_Right * translation.x + m_Up * translation.y + m_Normal * alongNormal;
	}

	template <typename T>
	Rect3<T> Rect3<T>::MovedLocal(const Vector3<T>& translation) const
	{
		return Rect3{
			m_LeftBot + m_Right * translation.x + m_Up * translation.y - m_Normal * translation.z,
			m_Right, m_Up, m_Normal, m_Size
		};
	}

	template <typename T>
	Rect3<T> Rect3<T>::Compressed(const Vector2<T>& compression, T alongNormal) const
	{
		return {
			m_LeftBot + m_Right * compression.x / 2 + m_Up * compression.y / 2 + m_Normal * alongNormal,
			m_Right, m_Up, m_Normal, m_Size - compression };
	}

	template <typename T>
	Rect3<T> Rect3<T>::Expanded(const Vector2<T>& expansion, T alongNormal) const
	{
		return {
			m_LeftBot - m_Right * expansion.x / 2 - m_Up * expansion.y / 2 + m_Normal * alongNormal,
			m_Right, m_Up, m_Normal, m_Size + expansion };
	}

	template <typename T>
	Rect3<T> Rect3<T>::MakeChild(const Vector2<T>& offset, const Vector2<T>& size) const
	{
		return Rect3{
			m_LeftBot + m_Right * offset.x + m_Up * offset.y,
			m_Right, m_Up, m_Normal, size
		};
	}

	template <typename T>
	Rect3<T> Rect3<T>::FromXyCenter(const Vector2<T>& center, const Vector2<T>& size, T depth)
	{
		return{
			{center.x - size.x / 2, center.y - size.y / 2, depth},
			{1,0,0}, {0,1,0}, {0,0,-1}, size };
	}
}
