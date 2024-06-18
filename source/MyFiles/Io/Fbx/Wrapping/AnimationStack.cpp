#include "AnimationStack.h"

#include "Io/Fbx/Reading/FbxElement.h"
#include "Io/Fbx/Reading/Properties70.h"
#include "Io/Fbx/Reading/Properties/FbxPropPrimitive.h"

MyEngine::Io::Fbx::Wrapping::AnimationStack::AnimationStack(Reading::FbxElement& object)
	: Id{ object.GetProperty(0).AsPrimitive<int64_t>().GetValue() }
	, Name{ object.GetProperty(1).AsString() }
{
	const Reading::Properties70 properties{ object.GetChild(0) };
	LocalStart = properties.GetUInt64("LocalStart", 0);
	LocalStop = properties.GetUInt64("LocalStop");
	ReferenceStart = properties.GetUInt64("ReferenceStart", 0);
	ReferenceStop = properties.GetUInt64("ReferenceStop");
}

void MyEngine::Io::Fbx::Wrapping::AnimationStack::AddAnimationLayer(const AnimationLayer& animationLayer)
{
	m_AnimationLayers.Add(&animationLayer);
}
