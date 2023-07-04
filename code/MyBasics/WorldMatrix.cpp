#include "WorldMatrix.h"

MyEngine::Float3 MyEngine::Game::WorldMatrix::GetPosition(const Float4X4& world)
{
	return { world[0][3], world[1][3], world[2][3] };
}

float MyEngine::Game::WorldMatrix::GetXPosition(const Float4X4& world)
{
	return world[0][3];
}

float MyEngine::Game::WorldMatrix::GetYPosition(const Float4X4& world)
{
	return world[1][3];
}

float MyEngine::Game::WorldMatrix::GetZPosition(const Float4X4& world)
{
	return world[2][3];
}

void MyEngine::Game::WorldMatrix::SetRotation(Float4X4& world, const Float3& forward)
{
	const Float3 right{ Float3{0,1,0}.Cross(forward).Normalized() };
	const Float3 up{ forward.Cross(right).Normalized() };
	world.SetRow0(right);
	world.SetRow1(up);
	world.SetRow2(forward);
}

MyEngine::Float3 MyEngine::Game::WorldMatrix::GetRight(const Float4X4& world)
{
	return { world[0][0], world[1][0], world[2][0] };
}

MyEngine::Float3 MyEngine::Game::WorldMatrix::GetUp(const Float4X4& world)
{
	return { world[0][1], world[1][1], world[2][1] };
}

MyEngine::Float3 MyEngine::Game::WorldMatrix::GetForward(const Float4X4& world)
{
	return { world[0][2], world[1][2], world[2][2] };
}

void MyEngine::Game::WorldMatrix::SetPosition(Float4X4& world, const Float3& position)
{
	world.SetRow3(position);
}

MyEngine::Float4X4 MyEngine::Game::WorldMatrix::Translation(const Float3& translation)
{
	Float4X4 m{ Float4X4::GetIdentity() };
	SetPosition(m, translation);
	return m;
}
