#pragma once
#include "JointsTimeValues.h"

namespace MyEngine
{
namespace Animations
{
struct JointCacheData {

	struct Float3Data {
		Float3 Begin;
		Float3 Delta;
	};
	struct QuaternionData {
		Quaternion Begin;
		Quaternion End;
		float Angle{};
		float Denom{};
		float InvDuration{};
	};

	template<typename T>
	struct Property {
		float BeginTime{};
		float EndTime{};
		unsigned IEndValue{};

		T Data;

		template<unsigned IProperty>
		void Init(const JointsTimeValues& source, unsigned iJoint);

		bool NeedUpdate(float target) const;

		template<unsigned IProperty>
		void Update(const JointsTimeValues& source, unsigned iJoint, float time);

		template<unsigned IProperty>
		void UpdateTime(const JointsTimeValues& source, unsigned iJoint, float time);
		void UpdateValue(const JointsTimeValues& source);
	};

	Property<Float3Data> Position;
	Property<QuaternionData> Rotation;
	//Property<Float3Data> Scale; //scale never needed nor tested

	Float3 GetPosition(float time) const;
	Quaternion GetRotation(float time) const;
	//Float3 GetScale(float time) const;

	void Init(const JointsTimeValues& source, unsigned iJoint);
};

class CachedData
{
public:
	CachedData() = default;
	CachedData(const JointsTimeValues& source);

	const JointCacheData& Get(const JointsTimeValues& source, unsigned iJoint, float time);

private:
	Array<JointCacheData> m_Data;
};
template<typename T>
template<unsigned IProperty>
inline void JointCacheData::Property<T>::Init(const JointsTimeValues& source, unsigned iJoint)
{
	IEndValue = source.m_Lookup[iJoint * JointsTimeValues::NR_PROPERTIES + IProperty] + JointsTimeValues::SIZE_PROPERTY[IProperty];
	EndTime = source.m_Data[IEndValue];
	BeginTime = 0;

	UpdateValue(source);
}
template<typename T>
template<unsigned IProperty>
inline void JointCacheData::Property<T>::Update(const JointsTimeValues& source, unsigned iJoint, float time)
{
	UpdateTime<IProperty>(source, iJoint, time);
	UpdateValue(source);
}
template<typename T>
template<unsigned IProperty>
inline void JointCacheData::Property<T>::UpdateTime(const JointsTimeValues& source, unsigned iJoint, float time)
{
	if (time < BeginTime)
	{
		//time went backwards, just search from begin (not really supported to go in reverse)
		IEndValue = source.m_Lookup[iJoint * JointsTimeValues::NR_PROPERTIES + IProperty];
	}

	const float* pData{ &source.m_Data[IEndValue] + JointsTimeValues::SIZE_PROPERTY[IProperty] };
	while (true)
	{
		const float& sourceTime{ *pData };
		if (sourceTime >= time)
		{
			//found next
			BeginTime = *(pData - JointsTimeValues::SIZE_PROPERTY[IProperty]);
			EndTime = sourceTime;
			IEndValue = static_cast<unsigned>(pData - source.m_Data.GetData());
			return;
		}
		else
		{
			if (sourceTime < EndTime)
			{
				//Out of bounds/into next property
				pData = &source.m_Data[source.m_Lookup[iJoint * JointsTimeValues::NR_PROPERTIES + IProperty]];
				EndTime = *pData;
			}
			pData += JointsTimeValues::SIZE_PROPERTY[IProperty];
		}
	}
}
template<typename T>
inline bool JointCacheData::Property<T>::NeedUpdate(float target) const
{
	return target < BeginTime || target > EndTime;
}
template<>
inline void JointCacheData::Property<JointCacheData::Float3Data>::UpdateValue(const JointsTimeValues& source)
{
	const float* pData{ &source.m_Data[IEndValue] };
	Data.Begin.x = pData[-3];
	Data.Begin.y = pData[-2];
	Data.Begin.z = pData[-1];

	Data.Delta.x = pData[1];
	Data.Delta.y = pData[2];
	Data.Delta.z = pData[3];
	Data.Delta = (Data.Delta - Data.Begin) / (EndTime - BeginTime);
}
template<>
inline void JointCacheData::Property<JointCacheData::QuaternionData>::UpdateValue(const JointsTimeValues& source)
{
	const float* pData{ &source.m_Data[IEndValue] };

	//Set time
	Data.InvDuration = 1.f / (EndTime - BeginTime);

	//Set values
	Data.Begin.Xyz.x = pData[-4];
	Data.Begin.Xyz.y = pData[-3];
	Data.Begin.Xyz.z = pData[-2];
	Data.Begin.W = pData[-1];

	Data.End.Xyz.x = pData[1];
	Data.End.Xyz.y = pData[2];
	Data.End.Xyz.z = pData[3];
	Data.End.W = pData[4];

	//coming from Quaternion::Slerp
	float dot{ Quaternion::Dot(Data.Begin, Data.End) };
	if (abs(dot) >= 1)
	{
		Data.InvDuration = 0;
		Data.Angle = Constants::PI_DIV2_S;
		Data.Denom = 1;
	}
	else
	{
		Data.Angle = acosf(dot);
		Data.Denom = 1.f / sinf(Data.Angle);
	}
}
inline void JointCacheData::Init(const JointsTimeValues& source, unsigned iJoint)
{
	Position.Init<0>(source, iJoint);
	Rotation.Init<1>(source, iJoint);
}
}
}