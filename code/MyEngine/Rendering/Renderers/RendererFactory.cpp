#include "pch.h"
#include "RendererFactory.h"

Rendering::RendererFactory::SimpleRenderer* Rendering::RendererFactory::CreateSimpleRenderer(App::Wrappers::Dx::Gpu& gpu,
	Game::Camera::Camera& camera)
{
	return new SimpleRenderer(gpu, camera, App::Resources::GetGlobalShaderPath(L"lambertCamDir.hlsl"));
}

Rendering::RendererFactory::WireframeRenderer* Rendering::RendererFactory::CreateWireframeRenderer(
	App::Wrappers::Dx::Gpu& gpu, Game::Camera::Camera& camera)
{
	return new WireframeRenderer(gpu, camera, App::Resources::GetGlobalShaderPath(L"shader.hlsl"), true);
}

Rendering::RendererFactory::SimpleTexRenderer* Rendering::RendererFactory::CreateSimpleTexRenderer(
	App::Wrappers::Dx::Gpu& gpu, Game::Camera::Camera& camera)
{
	return new SimpleTexRenderer(gpu, camera, App::Resources::GetGlobalShaderPath(L"lambertCamDir_texture.hlsl"), App::Resources::GetLocalResourcePath(L"Textures/FantasyRivals_Texture_01_A.png"));
}
