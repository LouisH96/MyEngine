#include "pch.h"
#include "Cube.h"

Cube::Cube(const Transform& transform, const Float3& size)
	: m_Transform{ transform }
	, m_Size{ size }
{
}

Cube::Cube(
	const Float3& leftBot, const Float3& right, const Float3& up, const Float3& forward,
	const Float3& size)
	: m_Transform{ leftBot, {} }
	, m_Size{ size }
{
}

Float3 Cube::GetRightBotBack() const
{
	return m_Transform.Position
		+ m_Transform.Rotation.GetRight() * m_Size.x
		+ m_Transform.Rotation.GetForward() * m_Size.z;
}

Float3 Cube::GetRightBotFront() const
{
	return m_Transform.Position
		+ m_Transform.Rotation.GetRight() * m_Size.x;
}

Float3 Cube::GetLeftBotBack() const
{
	return m_Transform.Position
		+ m_Transform.Rotation.GetForward() * m_Size.z;
}

Float3 Cube::GetLeftTopFront() const
{
	return m_Transform.Position
		+ m_Transform.Rotation.GetUp() * m_Size.y;
}

Float3 Cube::GetCenter() const
{
	return m_Transform.Position
		+ m_Transform.Rotation.GetRight() * m_Size.x * .5f
		+ m_Transform.Rotation.GetUp() * m_Size.y * .5f
		+ m_Transform.Rotation.GetForward() * m_Size.z * .5f;
}
