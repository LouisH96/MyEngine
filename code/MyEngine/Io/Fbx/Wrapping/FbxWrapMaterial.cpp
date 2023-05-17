#include "pch.h"
#include "FbxWrapMaterial.h"

#include "Io/Fbx/Reading/FbxObject.h"
#include "Io/Fbx/Reading/Properties/FbxPropPrimitive.h"

Io::Fbx::Wrapping::FbxWrapMaterial::FbxWrapMaterial(Reading::FbxObject& object)
	: Id{ object.GetProperty(0)->AsPrimitive<int64_t>().GetValue() }
{

}

void Io::Fbx::Wrapping::FbxWrapMaterial::AddModel(const Model& model)
{
	m_Models.Add(&model);
}

void Io::Fbx::Wrapping::FbxWrapMaterial::AddTexture(const FbxWrapTexture& texture)
{
	m_Textures.Add(&texture);
}
