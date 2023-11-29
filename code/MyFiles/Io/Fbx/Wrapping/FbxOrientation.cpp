#include "FbxOrientation.h"

#include "Logger/Logger.h"
#include "String/Convert.h"

using namespace MyEngine;
using namespace Io::Fbx::Wrapping;

FbxOrientation::FbxOrientation(int upAxis)
	: m_UpAxis{ upAxis }
	, m_IsLeftHanded{ false }
{
	Logger::Print("UpAxis", upAxis);

	if (m_UpAxis == 1 || m_UpAxis == 0)
	{
		//fbx is left-handed
		//thus need to rotate 180 degrees around y-axis
		//  z part will be 'automatically' applied
		//  x need to be mirrored
		m_XSign = -m_XSign;
		return;
	}
	if (m_UpAxis == 2)
	{
		m_YOffset = 2;
		m_ZOffset = 1;
		m_ZSign = -m_ZSign;

		//m_XSign = -m_XSign;
		m_IsLeftHanded = true;
		return;
	}

	Logger::PrintError("[FbxOrientation] current upAxis(" + Convert::ToString(m_UpAxis) + ") orientation not supported yet");
}

Float3 FbxOrientation::ConvertPoint(const double* pPoint) const
{
	return {
		m_XSign * static_cast<float>(pPoint[m_XOffset]),
		m_YSign * static_cast<float>(pPoint[m_YOffset]),
		m_ZSign * static_cast<float>(pPoint[m_ZOffset])
	};
}

Float3 FbxOrientation::ConvertPoint(const Float3& point) const
{
	return {
		m_XSign * point[m_XOffset],
		m_YSign * point[m_YOffset],
		m_ZSign * point[m_ZOffset],
	};
}

Float3 FbxOrientation::ConvertRotation(const Float3& rotation) const
{
	if (m_IsLeftHanded)
		return rotation;

	return {
		rotation.x,
		-rotation.y,
		-rotation.z
	};
}
