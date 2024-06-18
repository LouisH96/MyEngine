#pragma once

namespace MyEngine
{
	class Hexagon
	{
	public:
		explicit Hexagon(float outerRadius);

		float GetOuterRadius() const;
		float GetInnerRadius() const;

		static Hexagon FromOuterRadius(float outerRadius);
		static Hexagon FromInnerRadius(float innerRadius);

		static constexpr unsigned NR_POINTS = 6;
		static constexpr unsigned NR_SIDES = 6;

	private:
		float m_OuterRadius;
	};
}
