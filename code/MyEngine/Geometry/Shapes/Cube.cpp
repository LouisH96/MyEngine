#include "pch.h"
#include "Cube.h"

using namespace MyEngine;

Cube::Cube() : m_Origin{ 0.f }, m_Size{ 0.f } {}
Cube::Cube(Float3 origin, Float3 size) : m_Origin{ origin }, m_Size{ size } {}
Cube::Cube(Float3 origin, float size) : m_Origin{ origin }, m_Size{ size } {}
