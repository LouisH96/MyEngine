#include "pch.h"
#include "JointsTimeValues.h"

#include <Math\Math.h>

using namespace MyEngine::Animations;
using namespace Io::Fbx;

JointsTimeValues::JointsTimeValues(
	const List<FbxJoint>& joints,
	const Io::Fbx::FbxAnimation& animation,
	const FbxAnimationLayer& animLayer)
{
	const uint64_t start{ animation.GetLocalStart() };
	const uint64_t end{ animation.GetLocalStop() };

	FillLookup(joints, animLayer, start, end);
	FillData(joints, animLayer, start, end);
}

Float3 JointsTimeValues::GetPosition(unsigned iJoint, float time) const
{
	const unsigned iLookup{ iJoint * NR_JOINT_PROPERTIES };
	return Find(m_Lookup[iLookup], m_Lookup[iLookup + 1], time);
}

Float3 JointsTimeValues::GetEulerDegrees(unsigned iJoint, float time) const
{
	const unsigned iLookup{ iJoint * NR_JOINT_PROPERTIES + 1 };
	return Find(m_Lookup[iLookup], m_Lookup[iLookup + 1], time);
}

Float3 JointsTimeValues::GetScale(unsigned iJoint, float time) const
{
	const unsigned iLookup{ iJoint * NR_JOINT_PROPERTIES + 2 };
	return Find(m_Lookup[iLookup], m_Lookup[iLookup + 1], time);
}

void JointsTimeValues::FillLookup(const List<FbxJoint>& joints, const FbxAnimationLayer& animLayer,
	uint64_t startTime, uint64_t endTime)
{
	SortedList<uint64_t> times{ 5 };

	m_Lookup = GetLookupSize(joints);
	unsigned iData{ 0 };
	unsigned* pLookup{ m_Lookup.GetData() };
	for (unsigned iJoint{ 0 }; iJoint < joints.GetSize(); ++iJoint)
	{
		const FbxJoint& joint{ joints[iJoint] };
		const FbxTransformCurve* pCurves{ joint.FindCurve(animLayer) };
		if (pCurves == nullptr)
		{
			for (unsigned iProperty{ 0 }; iProperty < NR_JOINT_PROPERTIES; ++iProperty)
			{
				*pLookup++ = iData;
				iData += TIME_VALUE_SIZE * 2; //at least begin & end
			}
			continue;
		}

		for (unsigned iProperty{ 0 }; iProperty < NR_JOINT_PROPERTIES; ++iProperty)
		{
			const FbxValueCurve<float>* pPropertyCurves{ pCurves->GetCurves(iProperty) };

			//count distinct timeValues
			times.Clear();
			times.TryAdd(startTime);
			times.TryAdd(endTime);
			AddTimes(times, pPropertyCurves, endTime);

			*pLookup++ = iData;
			iData += times.GetSize() * TIME_VALUE_SIZE;
		}
	}
	m_Lookup.Last() = iData;
}

void JointsTimeValues::FillData(const List<FbxJoint>& joints, const FbxAnimationLayer& animLayer,
	uint64_t startTime, uint64_t endTime)
{
	const double durationD{ static_cast<double>(endTime - startTime) };
	SortedList<uint64_t> times{};

	m_Data = Array<float>{ m_Lookup.Last() };

	float* pData{ m_Data.GetData() };
	for (unsigned iJoint{ 0 }; iJoint < joints.GetSize(); ++iJoint)
	{
		const FbxJoint& joint{ joints[iJoint] };
		const FbxTransformCurve* pCurves{ joint.FindCurve(animLayer) };

		//Assert
		const unsigned assertCurrent{ static_cast<unsigned>(pData - m_Data.GetData()) };
		const unsigned assertExpected{ m_Lookup[iJoint * PROPERTY_VALUE_SIZE] };
		if (assertCurrent != assertExpected)
			Logger::PrintWarning("[JointsTimeValues::FillData] lookup doesn't match iData");

		if (!pCurves)
		{
			Logger::Print("[JointsTimeValues::FillData] no curves");
			AddIdentityProperties(pData);
			AddIdentityProperties(pData);
			continue;
		}

		//For each property
		for (unsigned iProperty{ 0 }; iProperty < NR_JOINT_PROPERTIES; ++iProperty)
		{
			const FbxValueCurve<float>* pPropertyCurves{ pCurves->GetCurves(iProperty) };

			//Get all times
			times.Clear();
			times.TryAdd(startTime);
			times.TryAdd(endTime);
			AddTimes(times, pPropertyCurves, endTime);

			//Insert all times
			for (unsigned iTime{ 0 }; iTime < times.GetSize(); ++iTime)
			{
				const float time{ static_cast<float>(
					static_cast<double>(times[iTime] - startTime) / durationD) };
				if (time > 1.f)
					Logger::PrintWarning("[JointsTimeValues::FillData] time bigger than 1");
				*pData = time;
				pData += TIME_VALUE_SIZE;
			}
			pData -= TIME_VALUE_SIZE * times.GetSize();

			//for each dimension
			for (unsigned iDim{ 0 }; iDim < PROPERTY_VALUE_SIZE; ++iDim)
			{
				pData++;
				const FbxValueCurve<float>& dimValues{ pPropertyCurves[iDim] };

				//insert values
				for (unsigned iTime{ 0 }; iTime < times.GetSize(); ++iTime)
				{
					*pData = dimValues.ValueAtTime(times[iTime]);
					pData += TIME_VALUE_SIZE;
				}
				pData -= TIME_VALUE_SIZE * times.GetSize();
			}
			pData++;
			pData += TIME_VALUE_SIZE * (times.GetSize() - 1);
		}
	}
}

Float3 JointsTimeValues::Find(const unsigned iFirst, const unsigned iEnd, float targetTime) const
{
	//Sequential search
	unsigned iBefore{ iFirst };
	for (unsigned iData{ iFirst + TIME_VALUE_SIZE }; iData < iEnd; iData += TIME_VALUE_SIZE)
	{
		const float time{ m_Data[iData] };
		if (time <= targetTime)
			iBefore = iData;
	}

	unsigned iAfter{ iBefore };
	for (unsigned iData{ iBefore }; iData < iEnd; iData += TIME_VALUE_SIZE)
	{
		const float time{ m_Data[iData] };
		if (time >= targetTime)
		{
			iAfter = iData;
			break;
		}
	}

	const float beforeTime{ m_Data[iBefore] };
	const float afterTime{ m_Data[iAfter] };
	const Float3 beforeValue{ m_Data[iBefore + 1], m_Data[iBefore + 2], m_Data[iBefore + 3] };
	const Float3 afterValue{ m_Data[iAfter + 1], m_Data[iAfter + 2], m_Data[iAfter + 3] };

	const float alpha{ Float::Unlerp(targetTime, beforeTime, afterTime) };
	return Math::Lerp(alpha, beforeValue, afterValue);
}

void JointsTimeValues::AddTimes(SortedList<uint64_t>& times, const FbxValueCurve<float>* pCurves, uint64_t endTime)
{
	for (unsigned iDim{ 0 }; iDim < PROPERTY_VALUE_SIZE; ++iDim)
	{
		const FbxValueCurve<float>& propertyCurve{ pCurves[iDim] };
		const SortedArray<uint64_t>& propertyTimes{ propertyCurve.GetTimes() };
		for (unsigned iPropTime{ 0 }; iPropTime < propertyTimes.GetSize(); ++iPropTime)
		{
			const uint64_t time{ propertyTimes[iPropTime] };
			if (time > endTime)
				return;
			times.TryAdd(propertyTimes[iPropTime]);
		}
	}
}

const unsigned JointsTimeValues::GetLookupSize(const List<FbxJoint>& joints)
{
	return joints.GetSize() * NR_JOINT_PROPERTIES + 1;
}

void JointsTimeValues::AddIdentityProperties(float*& pData)
{
	*pData++ = 0; //time

	for (unsigned iDim{ 0 }; iDim < PROPERTY_VALUE_SIZE * 2; ++iDim)
		*pData++ = 0; //position | rotation

	for (unsigned iDim{ 0 }; iDim < PROPERTY_VALUE_SIZE; ++iDim)
		*pData++ = 1; //scale
}

