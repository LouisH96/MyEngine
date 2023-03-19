#include "pch.h"
#include "Sphere.h"

Math::Sphere::Sphere(const Float3& center, float radius)
	: m_Center{ center }
	, m_Radius{ radius }
{
}

void Math::Sphere::SetCenter(const Float3& center)
{
	m_Center = center;
}

void Math::Sphere::SetRadius(float radius)
{
	m_Radius = radius;
}
