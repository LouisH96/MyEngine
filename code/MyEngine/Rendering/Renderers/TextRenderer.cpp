#include "pch.h"
#include "TextRenderer.h"

#include <fstream>
#include "Framework/Resources.h"
#include "Io/Ttf/ContourOperations.h"
#include "Io/Ttf/FontRasterizer.h"
#include "Io/Ttf/TtfReader.h"

Rendering::TextRenderer::TextRenderer(Gpu& gpu)
	: m_pTextureRenderer(
		new TextureRenderer<V_PosUv, CB_CamMat>(gpu, Framework::Resources::GetGlobalShaderPath(L"unlitTexture.hlsl")))
	, m_Gpu(gpu)
{
}


Rendering::TextRenderer::~TextRenderer()
{
	delete m_pTextureRenderer;
}

void Rendering::TextRenderer::AddCharacterXy(const Math::Float3& offset, const Math::Float2& size,
	const Io::Ttf::FontRasterizer& rasterizer) const
{
	const float left{ offset.x };
	const float bot{ offset.y };
	const float right{ left + size.x };
	const float top{ bot + size.y };
	const float z{ offset.z };
	Array<V_PosUv> vertices{ 6 };
	vertices[0] = { {left,top,z},{0,0} };
	vertices[1] = { {right,top,z},{1,0} };
	vertices[2] = { {right,bot,z},{1,1} };
	vertices[3] = { {left,top,z},{0,0} };
	vertices[4] = { {right,bot,z},{1,1} };
	vertices[5] = { {left,bot,z},{0,1} };
	Mesh* pMesh = Mesh::Create(m_Gpu, vertices);
	Image* pImage = rasterizer.MakeImage({});
	Texture* pTexture = new Texture(m_Gpu, pImage);
	delete pImage;
	m_pTextureRenderer->AddMesh(pMesh, pTexture);
}

void Rendering::TextRenderer::Render(const Math::Float3& cameraPosition, const Math::Float4X4& viewProjection) const
{
	m_pTextureRenderer->Render(cameraPosition, viewProjection);
}
