#pragma once
#include <Io\Fbx\FbxClass.h>

namespace MyEngine
{
namespace Animations
{
class JointsTimeValues
{
public:
	JointsTimeValues() = default;
	JointsTimeValues(
		const List<Io::Fbx::FbxJoint>& joints,
		const Io::Fbx::FbxAnimation& animation,
		const Io::Fbx::FbxAnimationLayer& animLayer);

	Float3 GetPosition(unsigned iJoint, float time) const;
	Quaternion GetQuaternion(unsigned iJoint, float time) const;
	Float3 GetScale(unsigned iJoint, float time) const;

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
		SortedList<uint64_t>& times, uint64_t startTime, uint64_t stopEnd, float normalizeTime);

	template<unsigned TPropertySize>
	static const float* FindBefore(const float* pBegin, const float* pEnd, float time);

	template<unsigned TPropertySize>
	static const float* FindAfter(const float* pBegin, const float* pEnd, float time);
};
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

