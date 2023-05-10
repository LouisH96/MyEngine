#include "pch.h"
#include "Sphere.h"

Shapes::Sphere::Sphere(const Float3& center, float radius)
	: m_Center{ center }
	, m_Radius{ radius }
{
}

void Shapes::Sphere::SetCenter(const Float3& center)
{
	m_Center = center;
}

void Shapes::Sphere::SetRadius(float radius)
{
	m_Radius = radius;
}
