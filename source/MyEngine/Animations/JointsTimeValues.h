#pragma once
#include <Io\Fbx\FbxClass.h>
#include <Math\Constants.h>

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
	};

	Property<Float3Data> Position;
	Property<QuaternionData> Rotation;
	Property<Float3Data> Scale;

	Float3 GetPosition(float time) const;
	Quaternion GetRotation(float time) const;
	Float3 GetScale(float time) const;
};

class JointsTimeValues
{
public:
	JointsTimeValues() = default;
	JointsTimeValues(
		const List<Io::Fbx::FbxJoint>& joints,
		const Io::Fbx::FbxAnimation& animation,
		const Io::Fbx::FbxAnimationLayer& animLayer);

	unsigned GetNrJoints() const { return (m_Lookup.GetSize() - 1) / NR_PROPERTIES; }

	Float3 GetPosition(unsigned iJoint, float time) const;
	Quaternion GetQuaternion(unsigned iJoint, float time) const;
	Float3 GetScale(unsigned iJoint, float time) const;

	template<unsigned IProperty, typename TData>
	void SetInitCacheTime(JointCacheData::Property<TData>& property, unsigned iJoint) const;

	template<unsigned IProperty, typename TData>
	void UpdateCacheTime(JointCacheData::Property<TData>& property, unsigned iJoint, float targetTime) const;

	template<typename TData>
	void UpdateCacheValues(JointCacheData::Property<TData>& property) const;

private:
	static constexpr unsigned ORDER_PROPERTY_POSITION{ 0 };
	static constexpr unsigned ORDER_PROPERTY_ROTATION{ 1 };
	static constexpr unsigned ORDER_PROPERTY_SCALE{ 2 };
	static constexpr unsigned NR_PROPERTIES{ 3 };

	static constexpr unsigned SIZE_PROPERTY_VALUE_POSITION{ 3 };
	static constexpr unsigned SIZE_PROPERTY_VALUE_ROTATION{ 4 };
	static constexpr unsigned SIZE_PROPERTY_VALUE_SCALE{ 3 };
	static constexpr unsigned SIZE_PROPERTY_VALUE[]{
		SIZE_PROPERTY_VALUE_POSITION,
		SIZE_PROPERTY_VALUE_ROTATION,
		SIZE_PROPERTY_VALUE_SCALE,
	};

	static constexpr unsigned SIZE_PROPERTY_TIME{ 1 };
	static constexpr unsigned SIZE_PROPERTY_POSITION{
		SIZE_PROPERTY_VALUE_POSITION + SIZE_PROPERTY_TIME
	};
	static constexpr unsigned SIZE_PROPERTY_ROTATION{
		SIZE_PROPERTY_VALUE_ROTATION + SIZE_PROPERTY_TIME
	};
	static constexpr unsigned SIZE_PROPERTY_SCALE{
		SIZE_PROPERTY_VALUE_SCALE + SIZE_PROPERTY_TIME
	};
	static constexpr unsigned SIZE_PROPERTY[]{
		SIZE_PROPERTY_POSITION,
		SIZE_PROPERTY_ROTATION,
		SIZE_PROPERTY_SCALE,
	};

	/*
		m_Data: For each property(pos|rot|scale), for each joint: the list of Time(Float1)Values(Float3)
		m_Lookup: The start for each TimeValueList (for example: [5] = begin of Joint1-Scale TimeValues
	*/
	Array<float> m_Data;
	Array<unsigned> m_Lookup;

	void FillLookup(const List<Io::Fbx::FbxJoint>& joints, const Io::Fbx::FbxAnimationLayer& animLayer, uint64_t startTime, uint64_t endTime);
	void FillData(const List<Io::Fbx::FbxJoint>& joints, const Io::Fbx::FbxAnimationLayer& animLayer, uint64_t startTime, uint64_t endTime);

	Float3 FindFloat3(unsigned iFirst, unsigned iEnd, float time) const;
	Quaternion FindRotation(unsigned iFirst, unsigned iEnd, float time) const;

	//input is pointer to 3 curves
	static void AddTimes(SortedList<uint64_t>& times, const Io::Fbx::FbxValueCurve<float>* pCurves, uint64_t startTime, uint64_t endTime);
	static const unsigned GetLookupSize(const List<Io::Fbx::FbxJoint>& joints);
	static void AddIdentityProperties(float*& pData);
	static void Write(float*& pData, const Float3& vector);
	static void Write(float*& pData, const Quaternion& quaternion);

	template<unsigned IProperty>
	static void AddIdentityProperty(float*& pData);
	template<unsigned IProperty>
	static void AddIdentityValue(float*& pData);

	static void AddDefaultPropertyList(unsigned iProperty, float*& pData,
		const Io::Fbx::FbxTransformCurve* pTransformCurve,
		SortedList<uint64_t>& times, uint64_t startTime, uint64_t stopEnd, float normalizeTime);

	static void AddRotationPropertyList(float*& pData,
		const Io::Fbx::FbxTransformCurve* pTransformCurve,
		const Quaternion& preRotation, const Quaternion& postRotation,
		SortedList<uint64_t>& times, uint64_t startTime, uint64_t stopEnd, float normalizeTime);

	template<unsigned TPropertySize>
	static const float* FindBefore(const float* pBegin, const float* pEnd, float time);

	template<unsigned TPropertySize>
	static const float* FindAfter(const float* pBegin, const float* pEnd, float time);
};
template<unsigned IProperty, typename TData>
inline void JointsTimeValues::SetInitCacheTime(JointCacheData::Property<TData>& property, unsigned iJoint) const
{
	property.IEndValue = m_Lookup[iJoint * NR_PROPERTIES + IProperty] + SIZE_PROPERTY[IProperty];
	property.EndTime = m_Data[property.IEndValue];
	property.BeginTime = 0;
}
template<unsigned IProperty, typename TData>
inline void JointsTimeValues::UpdateCacheTime(JointCacheData::Property<TData>& property, unsigned iJoint, float targetTime) const
{
	const float* pData{ &m_Data[property.IEndValue] + SIZE_PROPERTY[IProperty] };

	while (true)
	{
		const float& time{ *pData };

		if (time >= targetTime)
		{
			//found next
			property.BeginTime = *(pData - SIZE_PROPERTY[IProperty]);
			property.EndTime = time;
			property.IEndValue = static_cast<unsigned>(pData - m_Data.GetData());
			return;
		}
		else
		{
			if (time < property.EndTime)
			{
				//Out of bounds/into next property
				pData = &m_Data[m_Lookup[iJoint * NR_PROPERTIES + IProperty]];
				property.EndTime = *pData;
			}
			pData += SIZE_PROPERTY[IProperty];
		}
	}
}
template<>
inline void JointsTimeValues::UpdateCacheValues(
	JointCacheData::Property<JointCacheData::Float3Data>& property) const
{
	const float* pData{ &m_Data[property.IEndValue] };
	property.Data.Begin.x = pData[-3];
	property.Data.Begin.y = pData[-2];
	property.Data.Begin.z = pData[-1];

	property.Data.Delta.x = pData[1];
	property.Data.Delta.y = pData[2];
	property.Data.Delta.z = pData[3];
	property.Data.Delta =
		(property.Data.Delta - property.Data.Begin) / (property.EndTime - property.BeginTime);
}
template<>
inline void JointsTimeValues::UpdateCacheValues(
	JointCacheData::Property<JointCacheData::QuaternionData>& property) const
{
	const float* pData{ &m_Data[property.IEndValue] };

	//Set time
	property.Data.InvDuration = 1.f / (property.EndTime - property.BeginTime);

	//Set values
	property.Data.Begin.Xyz.x = pData[-4];
	property.Data.Begin.Xyz.y = pData[-3];
	property.Data.Begin.Xyz.z = pData[-2];
	property.Data.Begin.W = pData[-1];

	property.Data.End.Xyz.x = pData[1];
	property.Data.End.Xyz.y = pData[2];
	property.Data.End.Xyz.z = pData[3];
	property.Data.End.W = pData[4];

	//coming from Quaternion::Slerp
	float dot{ Quaternion::Dot(property.Data.Begin,property.Data.End) };
	if (abs(dot) >= 1)
	{
		property.Data.InvDuration = 0;
		property.Data.Angle = Constants::PI_DIV2_S;
		property.Data.Denom = 1;
	}
	else
	{
		property.Data.Angle = acosf(dot);
		property.Data.Denom = 1.f / sinf(property.Data.Angle);
	}
}
template<unsigned IProperty>
inline void JointsTimeValues::AddIdentityProperty(float*& pData)
{
	*pData++ = 0.f; //animation-start
	AddIdentityValue<IProperty>(pData);
	*pData++ = 1.f; //animation-stop
	AddIdentityValue<IProperty>(pData);
}
template<unsigned TPropertySize>
inline const float* JointsTimeValues::FindBefore(const float* pData, const float* pEnd, float time)
{
	const float* pBefore{ pData };
	while (pData != pEnd)
	{
		if (*pData <= time)
			pBefore = pData;

		pData += TPropertySize;
	}
	return pBefore;
}
template<unsigned TPropertySize>
inline const float* JointsTimeValues::FindAfter(const float* pData, const float* pEnd, float time)
{
	while (pData != pEnd)
	{
		if (*pData >= time)
			return pData;
		pData += TPropertySize;
	}
	Logger::PrintError("[JointsTimeValues::FindAfter] time not found");
	return nullptr;
}
template<>
inline void JointsTimeValues::AddIdentityValue<JointsTimeValues::ORDER_PROPERTY_POSITION>(float*& pData)
{
	Write(pData, Float3{ 0,0,0 });
}
template<>
inline void JointsTimeValues::AddIdentityValue<JointsTimeValues::ORDER_PROPERTY_ROTATION>(float*& pData)
{
	Write(pData, Quaternion{});
}
template<>
inline void JointsTimeValues::AddIdentityValue<JointsTimeValues::ORDER_PROPERTY_SCALE>(float*& pData)
{
	Write(pData, Float3{ 1,1,1 });
}
}
}

