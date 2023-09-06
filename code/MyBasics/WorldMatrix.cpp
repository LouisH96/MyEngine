#include "WorldMatrix.h"

using namespace MyEngine;

Float3 Game::WorldMatrix::GetPosition(const Float4X4& world)
{
	return { world[0][3], world[1][3], world[2][3] };
}

float Game::WorldMatrix::GetXPosition(const Float4X4& world)
{
	return world[0][3];
}

float Game::WorldMatrix::GetYPosition(const Float4X4& world)
{
	return world[1][3];
}

float Game::WorldMatrix::GetZPosition(const Float4X4& world)
{
	return world[2][3];
}

Float2 Game::WorldMatrix::GetPositionXz(const Float4X4& world)
{
	return { world[0][3], world[2][3] };
}

void Game::WorldMatrix::SetPositionXz(Float4X4& world, const Float2& xz)
{
	world[0][3] = xz.x;
	world[2][3] = xz.y;
}

void Game::WorldMatrix::SetRotation(Float4X4& world, const Float3& forward)
{
	const Float3 right{ Float3{0,1,0}.Cross(forward).Normalized() };
	const Float3 up{ forward.Cross(right).Normalized() };
	world.SetRow0(right);
	world.SetRow1(up);
	world.SetRow2(forward);
}

Float3 Game::WorldMatrix::GetRight(const Float4X4& world)
{
	return { world[0][0], world[1][0], world[2][0] };
}

Float3 Game::WorldMatrix::GetUp(const Float4X4& world)
{
	return { world[0][1], world[1][1], world[2][1] };
}

Float3 Game::WorldMatrix::GetForward(const Float4X4& world)
{
	return { world[0][2], world[1][2], world[2][2] };
}

Float2 Game::WorldMatrix::GetForwardXz(const Float4X4& world)
{
	return { world[0][2], world[2][2] };
}

float Game::WorldMatrix::GetYaw(const Float4X4& world)
{
	return -atan2f(world[2][0], world[0][0]);
}

void Game::WorldMatrix::SetYaw(Float4X4& world, float yaw)
{
	const float c{ cosf(yaw) };
	const float s{ sinf(yaw) };
	world[0].Xyz() = { c, 0, s }; //xAxis.x, yAxis.x, zAxis.x
	world[2].Xyz() = { -s, 0, c }; //xAxis.z, yAxis.z, zAxis.z
}

void Game::WorldMatrix::SetPosition(Float4X4& world, const Float3& position)
{
	world.SetRow3(position);
}

Float4X4 Game::WorldMatrix::Translation(const Float3& translation)
{
	Float4X4 m{ Float4X4::GetIdentity() };
	SetPosition(m, translation);
	return m;
}

Float4X4 Game::WorldMatrix::Rotation(const Float3& forward)
{
	Float4X4 world{};
	const Float3 right{ Float3{0,1,0}.Cross(forward).Normalized() };
	const Float3 up{ forward.Cross(right).Normalized() };
	world.SetRow0(right);
	world.SetRow1(up);
	world.SetRow2(forward);
	world.Set(3, 3, 1);
	return world;
}

Float4X4 Game::WorldMatrix::Rotation(float yaw, float pitch)
{
	const float cosYaw{ cosf(yaw) };
	const float sinYaw{ sinf(yaw) };
	const float cosPitch{ cosf(pitch) };
	const float sinPitch{ sinf(pitch) };

	return Float4X4{
			Float4{cosYaw * cosPitch,  -sinPitch, -sinYaw, 0},
			Float4{sinPitch, cosPitch,0, 0},
			Float4{sinYaw * cosPitch, -sinPitch * sinYaw, cosYaw, 0},
			Float4{0,0,0,1}
	};
}

void Game::WorldMatrix::Translate(Float4X4& m, const Float3& translation)
{
	m[0].w += translation.x;
	m[1].w += translation.y;
	m[2].w += translation.z;
}

void Game::WorldMatrix::TranslateRelativeX(Float4X4& m, float translation)
{
	m[0][3] += m[0][0] * translation;
	m[1][3] += m[1][0] * translation;
	m[2][3] += m[2][0] * translation;
}

void Game::WorldMatrix::TranslateRelativeY(Float4X4& m, float translation)
{
	m[0][3] += m[0][1] * translation;
	m[1][3] += m[1][1] * translation;
	m[2][3] += m[2][1] * translation;
}

void Game::WorldMatrix::SetPitchRotation(Float4X4& m, float pitch)
{
	const float cosPitch{ cosf(pitch) };
	const float sinPitch{ sinf(pitch) };
	m[0].Xyz() = { 1,0,0 };
	m[1].Xyz() = { 0, cosPitch, -sinPitch };
	m[2].Xyz() = { 0, sinPitch, cosPitch };
}
