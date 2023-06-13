#include "pch.h"
#include "RendererFactory.h"
#include <Framework/Resources.h>


Rendering::RendererFactory::SimpleRenderer* Rendering::RendererFactory::CreateSimpleRenderer(Gpu& gpu, bool wireframe)
{
	return new SimpleRenderer(gpu, Framework::Resources::GetGlobalShaderPath(L"lambertCamDir.hlsl"), wireframe);
}

Rendering::RendererFactory::UnlitRenderer* Rendering::RendererFactory::CreateUnlitRenderer(
	Gpu& gpu, bool wireframe)
{
	return new UnlitRenderer(gpu, Framework::Resources::GetGlobalShaderPath(L"unlit.hlsl"), wireframe);
}

Rendering::RendererFactory::SimpleTexRenderer* Rendering::RendererFactory::CreateSimpleTexRenderer(Gpu& gpu)
{
	return new SimpleTexRenderer(Framework::Resources::GetGlobalShaderPath(L"LambertCam_Tex.hlsl"));
}
