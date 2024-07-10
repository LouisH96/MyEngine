#include "FbxOrientation.h"

#include "Model.h"

using namespace MyEngine;
using namespace Io::Fbx::Wrapping;

FbxOrientation::FbxOrientation(float scale)
	: m_Scale{ scale }
{
}

Float3 FbxOrientation::ConvertPoint(const double* pPoint) const
{
	return {
		-static_cast<float>(pPoint[0]) * m_Scale,
		static_cast<float>(pPoint[1]) * m_Scale,
		static_cast<float>(pPoint[2]) * m_Scale
	};
}

Float3 FbxOrientation::ConvertPoint(const Float3& point) const
{
	return {
		-point.x * m_Scale,
		point.y * m_Scale,
		point.z * m_Scale
	};
}

Float3 FbxOrientation::ConvertRotation(const Float3& rotation)
{
	return {
		rotation.x,
		-rotation.y,
		-rotation.z
	};
}

Transform FbxOrientation::MakeLocalTransform(const Model& model) const
{
	const Float3 preRotation{ ConvertRotation(model.GetPreRotation()) };
	const Float3 rotation{ ConvertRotation(model.GetLclRotation()) };
	const Float3 postRotation{ ConvertRotation(model.GetPostRotation()) };
	const Float3 translation{ ConvertPoint(model.GetLclTranslation()) };

	Transform preZYX{ {}, Quaternion::FromEulerDegrees({0,0, preRotation.z}) };
	preZYX = Transform::LocalToWorld({ {}, Quaternion::FromEulerDegrees({0, preRotation.y, 0}) }, preZYX);
	preZYX = Transform::LocalToWorld({ {}, Quaternion::FromEulerDegrees({preRotation.x,0,0}) }, preZYX);

	Transform postXYZ{ {}, Quaternion::FromEulerDegrees({-postRotation.x,0,0}) };
	postXYZ = Transform::LocalToWorld({ {}, Quaternion::FromEulerDegrees({0,-postRotation.y, 0}) }, postXYZ);
	postXYZ = Transform::LocalToWorld({ {}, Quaternion::FromEulerDegrees({0,0, -postRotation.z}) }, postXYZ);

	const Transform lclRotation{ {}, Quaternion::FromEulerDegrees(rotation) };

	Transform local{ translation, {} };
	local = Transform::LocalToWorld(preZYX, local);
	local = Transform::LocalToWorld(lclRotation, local);
	local = Transform::LocalToWorld(postXYZ, local);
	return local;
}
