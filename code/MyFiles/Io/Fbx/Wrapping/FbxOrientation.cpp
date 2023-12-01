#include "FbxOrientation.h"

#include "Logger/Logger.h"

using namespace MyEngine;
using namespace Io::Fbx::Wrapping;

FbxOrientation::FbxOrientation(int upAxis)
	: m_UpAxis{ upAxis }
	, m_IsLeftHanded{ false }
{
	m_XSign = -m_XSign;
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
	return {
		rotation.x,
		-rotation.y,
		-rotation.z
	};
}
