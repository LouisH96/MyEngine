#include "pch.h"
#include "TextRenderer.h"

#include "App/Resources.h"

Rendering::TextRenderer::TextRenderer(Gpu& gpu, Game::Camera& camera)
	: m_pTextureRenderer(new TextureRenderer<V_PosUv, CB_CamMat>(gpu, camera, 
	                                                             App::Resources::GetGlobalShaderPath(L"unlitTexture.hlsl")))
{
	Array<V_PosUv> vertices{ 6 };
	vertices[0] = { {0,1,0},{0,0} };
	vertices[1] = { {1,1,0},{1,0} };
	vertices[2] = { {1,0,0},{1,1} };
	vertices[3] = { {0,1,0},{0,0} };
	vertices[4] = { {1,0,0},{1,1} };
	vertices[5] = { {0,0,0},{0,1} };
	Mesh* pMesh = Mesh::Create(gpu, vertices);
	Texture* pTexture = new Texture(gpu);

	m_pTextureRenderer->AddMesh(pMesh, pTexture);
}

Rendering::TextRenderer::~TextRenderer()
{
	delete m_pTextureRenderer;
}

void Rendering::TextRenderer::Render() const
{
	m_pTextureRenderer->Render();
}
