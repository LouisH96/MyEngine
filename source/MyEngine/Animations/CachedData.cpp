#include "pch.h"
#include "CachedData.h"

#include "JointsTimeValues.h"

using namespace MyEngine::Animations;

CachedData::CachedData(const JointsTimeValues& source)
	: m_Data{ source.GetNrJoints() }
{
	for (unsigned iJoint{ 0 }; iJoint < m_Data.GetSize(); ++iJoint)
		m_Data[iJoint].Init(source, iJoint);
}

const JointCacheData& CachedData::Get(const JointsTimeValues& source, unsigned iJoint, float time)
{
	JointCacheData& data{ m_Data[iJoint] };

	if (data.Position.NeedUpdate(time))
		data.Position.Update<0>(source, iJoint, time);

	if (data.Rotation.NeedUpdate(time))
		data.Rotation.Update<1>(source, iJoint, time);

	return data;
}

Float3 JointCacheData::GetPosition(float time) const
{
	return Position.Data.Begin + Position.Data.Delta * (time - Position.BeginTime);
}

Quaternion JointCacheData::GetRotation(float time) const
{
	time = (time - Rotation.BeginTime) * Rotation.Data.InvDuration;

	return
		Rotation.Data.Begin * (sinf((1.f - time) * Rotation.Data.Angle) * Rotation.Data.Denom)
		+ Rotation.Data.End * (sinf(time * Rotation.Data.Angle) * Rotation.Data.Denom);
}

//Float3 JointCacheData::GetScale(float time) const
//{
//	return Scale.Data.Begin + Scale.Data.Delta * (time - Scale.BeginTime);
//}