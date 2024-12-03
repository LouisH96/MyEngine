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
	Float3 GetEulerDegrees(unsigned iJoint, float time) const;
	Float3 GetScale(unsigned iJoint, float time) const;

private:
	static constexpr unsigned NR_JOINT_PROPERTIES{ 3 };
	static constexpr unsigned PROPERTY_VALUE_SIZE{ 3 };
	static constexpr unsigned TIME_VALUE_SIZE{ 1 + PROPERTY_VALUE_SIZE }; //Time + Property
	/*
		m_Data: For each property(pos|rot|scale), for each joint: the list of Time(Float1)Values(Float3)
		m_Lookup: The start for each TimeValueList (for example: [5] = begin of Joint1-Scale TimeValues
	*/
	Array<float> m_Data;
	Array<unsigned> m_Lookup;

	void FillLookup(const List<Io::Fbx::FbxJoint>& joints, const Io::Fbx::FbxAnimationLayer& animLayer, uint64_t startTime, uint64_t endTime);
	void FillData(const List<Io::Fbx::FbxJoint>& joints, const Io::Fbx::FbxAnimationLayer& animLayer, uint64_t startTime, uint64_t endTime);

	Float3 Find(const unsigned iFirst, const unsigned iEnd, float time) const;

	//input is pointer to 3 curves
	static void AddTimes(SortedList<uint64_t>& times, const Io::Fbx::FbxValueCurve<float>* pCurves, uint64_t endTime);
	static const unsigned GetLookupSize(const List<Io::Fbx::FbxJoint>& joints);
	static void AddIdentityProperties(float*& pData);
};
}
}

