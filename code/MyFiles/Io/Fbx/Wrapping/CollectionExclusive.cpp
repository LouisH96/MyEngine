#include "CollectionExclusive.h"

#include "Io/Fbx/Reading/FbxObject.h"
#include "Io/Fbx/Reading/Properties/FbxPropPrimitive.h"

MyEngine::Io::Fbx::Wrapping::CollectionExclusive::CollectionExclusive(Reading::FbxObject& object)
	: Id{ object.GetProperty(0)->AsPrimitive<int64_t>().GetValue() }
	, String1{ object.GetProperty(1)->AsString() }
	, String2{ object.GetProperty(2)->AsString() }
{
}

void MyEngine::Io::Fbx::Wrapping::CollectionExclusive::AddModel(const Model& model)
{
	m_Models.Add(&model);
}
