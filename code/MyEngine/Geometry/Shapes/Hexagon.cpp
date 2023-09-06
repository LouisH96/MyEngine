#include "pch.h"
#include "Hexagon.h"

Hexagon::Hexagon(float outerRadius)
	: m_OuterRadius{ outerRadius }
{
}

float Hexagon::GetOuterRadius() const
{
	return m_OuterRadius;
}

float Hexagon::GetInnerRadius() const
{
	return sqrtf(3.f) * .5f * m_OuterRadius;
}

Hexagon Hexagon::FromOuterRadius(float outerRadius)
{
	return Hexagon{ outerRadius };
}

Hexagon Hexagon::FromInnerRadius(float innerRadius)
{
	return Hexagon{ 2 * innerRadius / sqrtf(3.f) };
}
