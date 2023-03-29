#include "pch.h"
#include "RendererFactory.h"

Rendering::RendererFactory::SimpleRenderer* Rendering::RendererFactory::CreateSimpleRenderer(Gpu& gpu,
	Game::Camera::Camera& camera, bool wireframe)
{
	return new SimpleRenderer(gpu, camera, App::Resources::GetGlobalShaderPath(L"lambertCamDir.hlsl"), wireframe);
}

Rendering::RendererFactory::WireframeRenderer* Rendering::RendererFactory::CreateWireframeRenderer(
	Gpu& gpu, Game::Camera::Camera& camera)
{
	return new WireframeRenderer(gpu, camera, App::Resources::GetGlobalShaderPath(L"shader.hlsl"), true);
}

Rendering::RendererFactory::SimpleTexRenderer* Rendering::RendererFactory::CreateSimpleTexRenderer(
	Gpu& gpu, Game::Camera::Camera& camera)
{
	return new SimpleTexRenderer(gpu, camera, App::Resources::GetGlobalShaderPath(L"lambertCamDir_texture.hlsl"));
}
