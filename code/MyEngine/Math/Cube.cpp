#include "pch.h"
#include "Cube.h"

Math::Cube::Cube() : m_Origin{ 0.f }, m_Size{ 0.f } {}
Math::Cube::Cube(Float3 origin, Float3 size) : m_Origin{ origin }, m_Size{ size } {}
Math::Cube::Cube(Float3 origin, float size) : m_Origin{ origin }, m_Size{ size } {}
