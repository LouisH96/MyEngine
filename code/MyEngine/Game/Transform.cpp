#include "pch.h"
#include "Transform.h"

Transform::Transform(const Float3& position, const Quaternion& rotation)
	: Position{ position }
	, Rotation{ rotation }
{
}

Transform::Transform(const Double4X4& matrix)
{
	Position = Float3{
		static_cast<float>(matrix[0].w),
		static_cast<float>(matrix[1].w),
		static_cast<float>(matrix[2].w)
	};
	Rotation = Quaternion{ matrix };
}

Transform::Transform(const Float4X4& matrix)
{
	Position = matrix.GetRow3().Xyz();
	Rotation = Quaternion{ matrix };
}

Float4X4 Transform::AsMatrix() const
{
	//https://danceswithcode.net/engineeringnotes/quaternions/quaternions.html
	const float q0{ Rotation.W };
	const float q1{ Rotation.Xyz.x };
	const float q2{ Rotation.Xyz.y };
	const float q3{ Rotation.Xyz.z };

	const float q12{ q1 * q1 };
	const float q22{ q2 * q2 };
	const float q32{ q3 * q3 };
	return {
		{1 - 2 * q22 - 2 * q32, 2 * q1 * q2 - 2 * q0 * q3, 2 * q1 * q3 + 2 * q0 * q2, Position.x},
		{2 * q1 * q2 + 2 * q0 * q3, 1 - 2 * q12 - 2 * q32, 2 * q2 * q3 - 2 * q0 * q1, Position.y},
		{2 * q1 * q3 - 2 * q0 * q2,  2 * q2 * q3 + 2 * q0 * q1, 1 - 2 * q12 - 2 * q22, Position.z},
		{0,0,0,1}
	};
}

Float4X4 Transform::GetTransposeInverse() const
{
	//not tested yet
	const float q0{ Rotation.W };
	const float q1{ Rotation.Xyz.x };
	const float q2{ Rotation.Xyz.y };
	const float q3{ Rotation.Xyz.z };

	const float q12{ q1 * q1 };
	const float q22{ q2 * q2 };
	const float q32{ q3 * q3 };
	return {
		{1 - 2 * q22 - 2 * q32, 2 * q1 * q2 - 2 * q0 * q3, 2 * q1 * q3 + 2 * q0 * q2, 0},
		{2 * q1 * q2 + 2 * q0 * q3, 1 - 2 * q12 - 2 * q32, 2 * q2 * q3 - 2 * q0 * q1, 0},
		{2 * q1 * q3 - 2 * q0 * q2,  2 * q2 * q3 + 2 * q0 * q1, 1 - 2 * q12 - 2 * q22, 0},
		{-Position.x, -Position.y, -Position.z,1}
	};
}

void Transform::LookAt(const Float3& target)
{
	Rotation = Quaternion::FromForward((target - Position).Normalized());
}

Float3 Transform::WorldToLocal(const Float3& worldPoint) const
{
	Float3 local{ worldPoint - Position };
	(-Rotation).RotatePoint(local);
	return local;
}

Float3 Transform::LocalToWorld(const Float3& localPoint) const
{
	return Rotation.GetRotatedPoint(localPoint) + Position;
}

void Transform::SetRelativeTo(const Transform& parent)
{
	Position = (-parent.Rotation).GetRotatedPoint(Position - parent.Position);;
	Rotation = (-parent.Rotation * Rotation).Normalized();
}

Transform Transform::GetRelativeTo(const Transform& parent) const
{
	return{
		(-parent.Rotation).GetRotatedPoint(Position - parent.Position),
		(-parent.Rotation * Rotation).Normalized()
	};
}

Transform Transform::WorldToLocal(const Transform& world, const Transform& parent)
{
	return{
		   (-parent.Rotation).GetRotatedPoint(world.Position - parent.Position),
		   (-parent.Rotation * world.Rotation).Normalized()
	};
}

Transform Transform::LocalToWorld(const Transform& local, const Transform& parent)
{
	return{
		parent.Rotation.GetRotatedPoint(local.Position) + parent.Position,
		(parent.Rotation * local.Rotation).Normalized()
	};
}
