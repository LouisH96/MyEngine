#include "Cube.h"

MyEngine::Math::Cube::Cube() : m_Origin{ 0.f }, m_Size{ 0.f } {}
MyEngine::Math::Cube::Cube(Float3 origin, Float3 size) : m_Origin{ origin }, m_Size{ size } {}
MyEngine::Math::Cube::Cube(Float3 origin, float size) : m_Origin{ origin }, m_Size{ size } {}
