#include "Connection.h"

#include <sstream>

#include "Io/Ascii/AsciiWriter.h"
#include "Io/Fbx/Reading/FbxObject.h"
#include "Io/Fbx/Reading/Properties/FbxPropPrimitive.h"

MyEngine::Io::Fbx::Wrapping::Connection::Connection(Reading::FbxObject& readerObject)
	: Relation{ readerObject.GetProperty(0)->AsString() }
	, ChildId{ readerObject.GetProperty(1)->AsPrimitive<int64_t>().GetValue() }
	, ParentId{ readerObject.GetProperty(2)->AsPrimitive<int64_t>().GetValue() }
{
	if (readerObject.GetNrProperties() > 3)
		readerObject.GetProperty(3)->AsString();
}

void MyEngine::Io::Fbx::Wrapping::Connection::Print(int nrTabs) const
{
	std::stringstream ss{};
	Ascii::AsciiWriter tabs{ ss, nrTabs };

	tabs << "ChildId: " << ChildId << std::endl;
	tabs << "ParentId: " << ParentId << std::endl;
	tabs << "Relation: " << Relation << std::endl;
	if (Relation == "OP")
		tabs << "Property: " << Property << std::endl;

	std::cout << ss.str();
}
