#include "pch.h"
#include "RendererFactory.h"
#include <Framework/Resources.h>

Rendering::RendererFactory::SimpleRenderer* Rendering::RendererFactory::CreateSimpleRenderer(bool wireframe)
{
	return new SimpleRenderer(Framework::Resources::GetGlobalShaderPath(L"lambertCamDir.hlsl"), wireframe);
}

Rendering::RendererFactory::UnlitRenderer* Rendering::RendererFactory::CreateUnlitRenderer(bool wireframe)
{
	return new UnlitRenderer(Framework::Resources::GetGlobalShaderPath(L"unlit.hlsl"), wireframe);
}

Rendering::RendererFactory::SimpleTexRenderer* Rendering::RendererFactory::CreateSimpleTexRenderer()
{
	return new SimpleTexRenderer(Framework::Resources::GetGlobalShaderPath(L"LambertCam_Tex.hlsl"));
}
