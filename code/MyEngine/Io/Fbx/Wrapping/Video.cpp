#include "pch.h"
#include "Video.h"

#include "Io/Fbx/Reading/FbxObject.h"
#include "Io/Fbx/Reading/Properties/FbxPropPrimitive.h"

Io::Fbx::Wrapping::Video::Video(Reading::FbxObject& object)
	: Id{ object.GetProperty(0)->AsPrimitive<int64_t>().GetValue() }
{
}

void Io::Fbx::Wrapping::Video::AddTexture(const FbxWrapTexture& texture)
{
	m_Textures.Add(&texture);
}
