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
	const unsigned iLookup{ iJoint * NR_PROPERTIES };
	return FindFloat3(m_Lookup[iLookup], m_Lookup[iLookup + 1], time);
}

Quaternion JointsTimeValues::GetQuaternion(unsigned iJoint, float time) const
{
	const unsigned iLookup{ iJoint * NR_PROPERTIES + ORDER_PROPERTY_ROTATION };
	return FindRotation(m_Lookup[iLookup], m_Lookup[iLookup + 1], time);
}

Float3 JointsTimeValues::GetScale(unsigned iJoint, float time) const
{
	const unsigned iLookup{ iJoint * NR_PROPERTIES + ORDER_PROPERTY_SCALE };
	return FindFloat3(m_Lookup[iLookup], m_Lookup[iLookup + 1], time);
}

void JointsTimeValues::FillLookup(const List<FbxJoint>& joints, const FbxAnimationLayer& animLayer,
	uint64_t startTime, uint64_t endTime)
{
	SortedList<uint64_t> times{ 16 };

	m_Lookup = GetLookupSize(joints);
	unsigned iData{ 0 };
	unsigned* pLookup{ m_Lookup.GetData() };
	for (unsigned iJoint{ 0 }; iJoint < joints.GetSize(); ++iJoint)
	{
		const FbxJoint& joint{ joints[iJoint] };
		const FbxTransformCurve* pCurves{ joint.FindCurve(animLayer) };
		if (pCurves == nullptr)
		{
			for (unsigned iProperty{ 0 }; iProperty < NR_PROPERTIES; ++iProperty)
			{
				*pLookup++ = iData;
				iData += SIZE_PROPERTY[iProperty] * 2; //at least begin & end
			}
			continue;
		}

		for (unsigned iProperty{ 0 }; iProperty < NR_PROPERTIES; ++iProperty)
		{
			const FbxValueCurve<float>* pPropertyCurves{ pCurves->GetCurves(iProperty) };

			//count distinct timeValues
			AddTimes(times, pPropertyCurves, startTime, endTime);

			*pLookup++ = iData;
			iData += times.GetSize() * SIZE_PROPERTY[iProperty];
		}
	}
	m_Lookup.Last() = iData;
}

void JointsTimeValues::FillData(const List<FbxJoint>& joints, const FbxAnimationLayer& animLayer,
	uint64_t startTime, uint64_t endTime)
{
	const float normalizeTime{ 1.f / static_cast<float>(endTime - startTime) };
	SortedList<uint64_t> times{};

	m_Data = Array<float>{ m_Lookup.Last() };

	float* pData{ m_Data.GetData() };
	for (unsigned iJoint{ 0 }; iJoint < joints.GetSize(); ++iJoint)
	{
		const FbxJoint& joint{ joints[iJoint] };
		const FbxTransformCurve* pCurves{ joint.FindCurve(animLayer) };

		//Assert
		const unsigned assertCurrent{ static_cast<unsigned>(pData - m_Data.GetData()) };
		const unsigned assertExpected{ m_Lookup[iJoint * NR_PROPERTIES] };
		if (assertCurrent != assertExpected)
			Logger::PrintWarning("[JointsTimeValues::FillData] lookup doesn't match iData");

		if (!pCurves)
		{
			Logger::Print("[JointsTimeValues::FillData] no curves");
			AddIdentityProperties(pData);
			continue;
		}

		AddDefaultPropertyList(0, pData, pCurves,
			times, startTime, endTime, normalizeTime);

		AddRotationPropertyList(pData, pCurves, 
			joint.GetPreRotationZYX().Rotation, joint.GetPostRotationXYZ().Rotation,
			times, startTime, endTime, normalizeTime);

		AddDefaultPropertyList(2, pData, pCurves,
			times, startTime, endTime, normalizeTime);
	}
}

Float3 JointsTimeValues::FindFloat3(unsigned iFirst, unsigned iEnd, float targetTime) const
{
	//Sequential search
	const float* pBefore{ FindBefore<4>(&m_Data[iFirst], &m_Data[iEnd], targetTime) };
	const float* pAfter{ FindAfter<4>(pBefore, &m_Data[iEnd], targetTime) };

	const Float3 beforeValue{ pBefore[1], pBefore[2], pBefore[3] };
	const Float3 afterValue{ pAfter[1], pAfter[2], pAfter[3] };

	const float alpha{ Float::Unlerp(targetTime, *pBefore, *pAfter) };
	return Math::Lerp(alpha, beforeValue, afterValue);
}

Quaternion JointsTimeValues::FindRotation(unsigned iFirst, unsigned iEnd, float time) const
{
	const float* pBefore{ FindBefore<5>(&m_Data[iFirst], &m_Data[iEnd], time) };
	const float* pAfter{ FindAfter<5>(pBefore, &m_Data[iEnd], time) };

	const Quaternion beforeValue{ {pBefore[1],pBefore[2], pBefore[3]}, pBefore[4] };
	const Quaternion afterValue{ {pAfter[1],pAfter[2], pAfter[3]}, pAfter[4] };
	const float alpha{ Float::Unlerp(time, *pBefore, *pAfter) };

	return Quaternion::Slerp(beforeValue, afterValue, alpha);
}

void JointsTimeValues::AddTimes(
	SortedList<uint64_t>& times,
	const FbxValueCurve<float>* pCurves,
	uint64_t startTime, uint64_t endTime)
{
	times.Clear();
	times.TryAdd(startTime);
	times.TryAdd(endTime);
	for (unsigned iDim{ 0 }; iDim < 3; ++iDim)
	{
		const FbxValueCurve<float>& propertyCurve{ pCurves[iDim] };
		const SortedArray<uint64_t>& propertyTimes{ propertyCurve.GetTimes() };
		for (unsigned iPropTime{ 0 }; iPropTime < propertyTimes.GetSize(); ++iPropTime)
		{
			const uint64_t time{ propertyTimes[iPropTime] };
			if (time >= endTime)
				break;
			if (time > startTime)
				times.TryAdd(propertyTimes[iPropTime]);
		}
	}
}

const unsigned JointsTimeValues::GetLookupSize(const List<FbxJoint>& joints)
{
	return joints.GetSize() * NR_PROPERTIES + 1;
}

void JointsTimeValues::AddIdentityProperties(float*& pData)
{
	AddIdentityProperty<ORDER_PROPERTY_POSITION>(pData);
	AddIdentityProperty<ORDER_PROPERTY_ROTATION>(pData);
	AddIdentityProperty<ORDER_PROPERTY_SCALE>(pData);
}

void JointsTimeValues::Write(float*& pData, const Float3& vector)
{
	*pData++ = vector.x;
	*pData++ = vector.y;
	*pData++ = vector.z;
}

void JointsTimeValues::Write(float*& pData, const Quaternion& quaternion)
{
	*pData++ = quaternion.Xyz.x;
	*pData++ = quaternion.Xyz.y;
	*pData++ = quaternion.Xyz.z;
	*pData++ = quaternion.W;
}

void JointsTimeValues::AddDefaultPropertyList(
	unsigned iProperty, float*& pData,
	const FbxTransformCurve* pTransformCurve,
	SortedList<uint64_t>& times, uint64_t startTime, uint64_t stopTime, float normalizeTime)
{
	const FbxValueCurve<float>* pCurves{ pTransformCurve->GetCurves(iProperty) };
	AddTimes(times, pCurves, startTime, stopTime);

	//Insert all times
	for (unsigned iTime{ 0 }; iTime < times.GetSize(); ++iTime)
	{
		const float time{ static_cast<float>((times[iTime] - startTime)) * normalizeTime };
		if (time > 1.f)
			Logger::PrintWarning("[JointsTimeValues::FillData] time bigger than 1");
		*pData = time;
		pData += SIZE_PROPERTY[iProperty];
	}
	pData -= SIZE_PROPERTY[iProperty] * times.GetSize();

	//for each dimension
	for (unsigned iDim{ 0 }; iDim < 3; ++iDim)
	{
		pData++;
		const FbxValueCurve<float>& dimValues{ pCurves[iDim] };

		//insert values
		for (unsigned iTime{ 0 }; iTime < times.GetSize(); ++iTime)
		{
			*pData = dimValues.ValueAtTime(times[iTime]);
			pData += SIZE_PROPERTY[iProperty];
		}
		pData -= SIZE_PROPERTY[iProperty] * times.GetSize();
	}
	pData++;
	pData += SIZE_PROPERTY[iProperty] * (times.GetSize() - 1);
}

void JointsTimeValues::AddRotationPropertyList(
	float*& pData,
	const FbxTransformCurve* pTransformCurve,
	const Quaternion& preRotation, const Quaternion& postRotation,
	SortedList<uint64_t>& times, uint64_t startTime, uint64_t stopTime, float normalizeTime)
{
	const FbxValueCurve<float>* pCurves{ pTransformCurve->GetRotationCurves() };
	AddTimes(times, pCurves, startTime, stopTime);

	//
	for (unsigned iTime{ 0 }; iTime < times.GetSize(); ++iTime)
	{
		//time
		const uint64_t& time{ times[iTime] };
		*pData++ = static_cast<float>(time - startTime) * normalizeTime;

		//value
		const Vector3 eulers{
			pCurves[0].ValueAtTime(time),
			pCurves[1].ValueAtTime(time),
			pCurves[2].ValueAtTime(time)
		};
		const Quaternion quaternion{
			preRotation * Quaternion::FromEulerDegrees(eulers) * postRotation };

		Write(pData, quaternion);
	}
}

