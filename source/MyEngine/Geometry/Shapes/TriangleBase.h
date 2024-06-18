#pragma once

namespace MyEngine
{
	template<typename Point>
	class TriangleBase
	{
	public:
		using Unit = Point;
		static constexpr unsigned NR_POINTS{ 3 };

		TriangleBase() = default;
		TriangleBase(const Point& p0, const Point& p1, const Point& p2);

		void SetPoint(unsigned idx, const Point& point);
		void SetPoint0(const Point& point);
		void SetPoint1(const Point& point);
		void SetPoint2(const Point& point);

		const Point& GetPoint(unsigned idx) const;
		const Point& GetPoint0() const;
		const Point& GetPoint1() const;
		const Point& GetPoint2() const;

		Point& GetPoint(unsigned idx);
		Point& GetPoint0();
		Point& GetPoint1();
		Point& GetPoint2();

	private:
		Point m_Points[NR_POINTS];
	};

	template<typename Point>
	inline TriangleBase<Point>::TriangleBase(const Point& p0, const Point& p1, const Point& p2)
		: m_Points{ p0,p1,p2 }
	{
	}

	template<typename Point>
	inline void TriangleBase<Point>::SetPoint(unsigned idx, const Point& point)
	{
		m_Points[idx] = point;
	}

	template<typename Point>
	inline void TriangleBase<Point>::SetPoint0(const Point& point)
	{
		m_Points[0] = point;
	}

	template<typename Point>
	inline void TriangleBase<Point>::SetPoint1(const Point& point)
	{
		m_Points[1] = point;
	}

	template<typename Point>
	inline void TriangleBase<Point>::SetPoint2(const Point& point)
	{
		m_Points[2] = point;
	}

	template<typename Point>
	inline const Point& TriangleBase<Point>::GetPoint(unsigned idx) const
	{
		return m_Points[idx];
	}

	template<typename Point>
	inline const Point& TriangleBase<Point>::GetPoint0() const
	{
		return m_Points[0];
	}

	template<typename Point>
	inline const Point& TriangleBase<Point>::GetPoint1() const
	{
		return m_Points[1];
	}

	template<typename Point>
	inline const Point& TriangleBase<Point>::GetPoint2() const
	{
		return m_Points[2];
	}

	template<typename Point>
	inline Point& TriangleBase<Point>::GetPoint(unsigned idx)
	{
		return m_Points[idx];
	}

	template<typename Point>
	inline Point& TriangleBase<Point>::GetPoint0()
	{
		return m_Points[0];
	}

	template<typename Point>
	inline Point& TriangleBase<Point>::GetPoint1()
	{
		return m_Points[1];
	}

	template<typename Point>
	inline Point& TriangleBase<Point>::GetPoint2()
	{
		return m_Points[2];
	}

}