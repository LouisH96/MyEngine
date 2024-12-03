#pragma once

#include <Io\Fbx\FbxJoint.h>

namespace MyEngine
{
namespace Animations
{
class JointsChildren
{
public:
	JointsChildren() = default;
	JointsChildren(const List<Io::Fbx::FbxJoint>& joints);

	//returns end
	const unsigned* GetIterator(unsigned iJoint, const unsigned*& iBegin);

	void Print(unsigned iJoint) const;

private:
	/*
		Lookup-Part: for each joint-idx: iBegin for child-id's
		Child-Id's: list of of the joint's child-ids
	*/
	Array<unsigned> m_Data;
};
}
}

