#include "pch.h"
#include "CubeAA.h"

using namespace MyEngine;

CubeAA::CubeAA() : m_Origin{ 0.f }, m_Size{ 0.f } {}
CubeAA::CubeAA(Float3 origin, Float3 size) : m_Origin{ origin }, m_Size{ size } {}
CubeAA::CubeAA(Float3 origin, float size) : m_Origin{ origin }, m_Size{ size } {}
