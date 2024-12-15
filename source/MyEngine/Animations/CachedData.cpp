#include "pch.h"
#include "CachedData.h"

#include "JointsTimeValues.h"

using namespace MyEngine::Animations;

CachedData::CachedData(const JointsTimeValues& source)
	: m_Data{ source.GetNrJoints() }
{
	for (unsigned iJoint{ 0 }; iJoint < m_Data.GetSize(); ++iJoint)
	{
		JointCacheData& joint{ m_Data[iJoint] };
		
		source.SetInitCacheTime<0>(joint.Position, iJoint);
		source.UpdateCacheValues(joint.Position);

		source.SetInitCacheTime<1>(joint.Rotation, iJoint);
		source.UpdateCacheValues(joint.Rotation);
	}
}

const JointCacheData& CachedData::Get(const JointsTimeValues& source, unsigned iJoint, float time)
{
	JointCacheData& data{ m_Data[iJoint] };

	if (time < data.Position.BeginTime || time > data.Position.EndTime)
	{
		source.UpdateCacheTime<0>(data.Position, iJoint, time);
		source.UpdateCacheValues(data.Position);
	}

	if (time < data.Rotation.BeginTime || time > data.Rotation.EndTime)
	{
		source.UpdateCacheTime<1>(data.Rotation, iJoint, time);
		source.UpdateCacheValues(data.Rotation);
	}

	//if (time < data.Scale.BeginTime || time > data.Scale.EndTime)
	//	source.CacheRotation(data, iJoint, time);

	return data;
}
