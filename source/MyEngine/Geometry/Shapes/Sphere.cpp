#include "pch.h"
#include "Sphere.h"

Sphere::Sphere(const Float3& center, float radius)
	: m_Center{ center }
	, m_Radius{ radius }
{
}

void Sphere::SetCenter(const Float3& center)
{
	m_Center = center;
}

void Sphere::SetRadius(float radius)
{
	m_Radius = radius;
}
