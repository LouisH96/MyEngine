#include "pch.h"
#include "FbxWrapTexture.h"

#include "Io/Fbx/Reading/FbxObject.h"
#include "Io/Fbx/Reading/Properties/FbxPropPrimitive.h"

Io::Fbx::Wrapping::FbxWrapTexture::FbxWrapTexture(Reading::FbxObject& object)
	: Id{ object.GetProperty(0)->AsPrimitive<int64_t>().GetValue() }
{
}

void Io::Fbx::Wrapping::FbxWrapTexture::AddLinkedVideo(const Video& video)
{
	m_LinkedVideos.Add(&video);
}
