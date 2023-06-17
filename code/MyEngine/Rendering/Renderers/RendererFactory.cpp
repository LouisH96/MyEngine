#include "pch.h"
#include "RendererFactory.h"
#include <Framework/Resources.h>

Rendering::RendererFactory::SimpleRenderer* Rendering::RendererFactory::CreateSimpleRenderer(bool wireframe)
{
	return new SimpleRenderer(Resources::GlobalShader(L"lambertCamDir.hlsl"), wireframe);
}

Rendering::RendererFactory::UnlitRenderer* Rendering::RendererFactory::CreateUnlitRenderer(bool wireframe)
{
	return new UnlitRenderer(Resources::GlobalShader(L"unlit.hlsl"), wireframe);
}

Rendering::RendererFactory::SimpleTexRenderer* Rendering::RendererFactory::CreateSimpleTexRenderer()
{
	return new SimpleTexRenderer(Resources::GlobalShader(L"LambertCam_Tex.hlsl"));
}
