#include "Video.h"

#include "Io/Fbx/Reading/FbxElement.h"
#include "Io/Fbx/Reading/Properties/FbxPropPrimitive.h"

MyEngine::Io::Fbx::Wrapping::Video::Video(Reading::FbxElement& object)
	: Id{ object.GetProperty(0).AsPrimitive<int64_t>().GetValue() }
{
}

void MyEngine::Io::Fbx::Wrapping::Video::AddTexture(const FbxWrapTexture& texture)
{
	m_Textures.Add(&texture);
}
