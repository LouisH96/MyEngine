#include "pch.h"
#include "Capsule.h"

Capsule::Capsule()
	: m_Bottom{ 0,0,0 }
	, m_Length{ 0 }
	, m_Radius{}
{
}

Capsule::Capsule(float length, float radius)
	: m_Bottom{ 0,0,0 }
	, m_Length{ length }
	, m_Radius{ radius }
{
}

Capsule::Capsule(const Float3& bottom, float length, float radius)
	: m_Bottom{ bottom }
	, m_Length{ length }
	, m_Radius{ radius }
{
}
