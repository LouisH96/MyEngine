#pragma once

#include "JointsTimeValues.h"

namespace MyEngine
{
namespace Animations
{

class CachedData
{
public:
	CachedData() = default;
	CachedData(const JointsTimeValues& source);

	const JointCacheData& Get(const JointsTimeValues& source, unsigned iJoint, float time);

private:

	Array<JointCacheData> m_Data;
};
}
}

