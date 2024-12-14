#include "pch.h"
#include "CachedData.h"

#include "JointsTimeValues.h"

using namespace MyEngine::Animations;

CachedData::CachedData(const JointsTimeValues& source)
	: m_Data{ source.GetNrJoints() }
{
	for (unsigned iJoint{ 0 }; iJoint < m_Data.GetSize(); ++iJoint)
		source.CacheData(m_Data[iJoint], iJoint, 0);
}

const JointCacheData& CachedData::Get(const JointsTimeValues& source, unsigned iJoint, float time)
{
	JointCacheData& data{ m_Data[iJoint] };

	if (time < data.Position.BeginTime || time > data.Position.EndTime)
		source.CachePosition(data, iJoint, time);

	if (time < data.Rotation.BeginTime || time > data.Rotation.EndTime)
		source.CacheRotation(data, iJoint, time);

	//if (time < data.Scale.BeginTime || time > data.Scale.EndTime)
	//	source.CacheRotation(data, iJoint, time);

	return data;
}
