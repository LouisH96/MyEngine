#include "pch.h"
#include "RendererFactory.h"
#include <Framework/Resources.h>


Rendering::RendererFactory::SimpleRenderer* Rendering::RendererFactory::CreateSimpleRenderer(Gpu& gpu,
                                                                                             Game::FocusPointCamera& camera, bool wireframe)
{
	return new SimpleRenderer(gpu, camera, Framework::Resources::GetGlobalShaderPath(L"lambertCamDir.hlsl"), wireframe);
}

Rendering::RendererFactory::UnlitRenderer* Rendering::RendererFactory::CreateUnlitRenderer(
	Gpu& gpu, Game::FocusPointCamera& camera, bool wireframe)
{
	return new UnlitRenderer(gpu, camera, Framework::Resources::GetGlobalShaderPath(L"unlit.hlsl"), wireframe);
}

Rendering::RendererFactory::SimpleTexRenderer* Rendering::RendererFactory::CreateSimpleTexRenderer(
	Gpu& gpu, Game::FocusPointCamera& camera)
{
	return new SimpleTexRenderer(gpu, camera, Framework::Resources::GetGlobalShaderPath(L"LambertCam_Tex.hlsl"));
}
