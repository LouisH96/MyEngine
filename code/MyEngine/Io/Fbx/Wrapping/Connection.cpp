#include "pch.h"
#include "Connection.h"

#include "Io/Fbx/Reading/FbxObject.h"
#include "Io/Fbx/Reading/Properties/FbxPropPrimitive.h"

Io::Fbx::Wrapping::Connection::Connection(Reading::FbxObject& readerObject)
	: Relation{ readerObject.GetProperty(0)->AsString() }
	, Id{ readerObject.GetProperty(1)->AsPrimitive<int64_t>().GetValue() }
	, ParentId{ readerObject.GetProperty(2)->AsPrimitive<int64_t>().GetValue() }
{
	if (readerObject.GetNrProperties() > 3)
		readerObject.GetProperty(3)->AsString();
}
