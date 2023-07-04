#include "pch.h"
#include "FpsDisplay.h"

#include <fstream>
#include <App/Win32/Window.h>
#include <Framework/Resources.h>
#include <Io/Ttf/FontRasterizer.h>
#include <Io/Ttf/Glyph.h>
#include <Io/Ttf/TtfReader.h>
#include <Rendering/State/Mesh.h>
#include <Rendering/State/Texture.h>
#include <Rendering/Structs/VertexTypes.h>

#undef min
#undef max

Rendering::FpsDisplay::FpsDisplay()
	: m_Shader(Resources::GlobalShader(L"screenSpace.hlsl"))
	, m_InputLayout(V_Pos2Uv::ELEMENTS, V_Pos2Uv::NR_ELEMENTS)
	, m_Vertices{ 6 }
{
	const std::wstring fontPath = Resources::Global(L"Fonts\\ShortBaby.ttf");
	std::ifstream stream{ fontPath, std::ios::binary };
	const Io::TtfReader reader{ stream };
	const Io::Ttf::Glyph glyphs[NR_NUMBERS]
	{
		reader.GetGlyph('0'), reader.GetGlyph('1'), reader.GetGlyph('2'), reader.GetGlyph('3'),
		reader.GetGlyph('4'), reader.GetGlyph('5'), reader.GetGlyph('6'), reader.GetGlyph('7'),
		reader.GetGlyph('8'), reader.GetGlyph('9'),
	};

	int16_t maxWidth = std::numeric_limits<int16_t>::min();
	for (int i = 0; i < NR_NUMBERS; i++)
		if (glyphs[i].GetSize().x > maxWidth) maxWidth = glyphs[i].GetSize().x;
	constexpr int maxWidthPixels = 32;
	const double pixelsPerUnit = maxWidthPixels / static_cast<double>(maxWidth);

	constexpr float margin = 10.f;
	m_ScreenSpaceMargin = {
		margin / Globals::pWindow->GetClientWidth(),
		margin / Globals::pWindow->GetClientHeight()
	};

	float xPos = -1.f + m_ScreenSpaceMargin.x;
	for (int i = 0; i < NR_NUMBERS; i++)
	{
		const Io::Ttf::Glyph& glyph{ glyphs[i] };
		const int pixelsInWidth{ static_cast<int>(ceil(glyph.GetSize().x * pixelsPerUnit)) };
		const int pixelsInHeight{ static_cast<int>(ceil(glyph.GetSize().y * pixelsPerUnit)) };

		const Io::Ttf::FontRasterizer rasterizer{ glyph, pixelsInWidth, pixelsInHeight };
		Image* pImage = rasterizer.MakeImage({ 1,1,0 });
		Texture* pTexture = new Texture(pImage);
		rasterizer.DeleteImage(pImage);

		m_Sizes[i] = {
			static_cast<float>(pixelsInWidth) / Globals::pWindow->GetClientWidth(),
			static_cast<float>(pixelsInHeight) / Globals::pWindow->GetClientHeight()
		};
		const Float2 screenSpacePos{
			xPos, 1.f - m_Sizes[i].y - m_ScreenSpaceMargin.y
		};
		xPos += m_Sizes[i].x;
		m_pTextures[i] = pTexture;
	}

	InitVertices(m_Vertices);
	m_pMesh = Mesh::Create(m_Vertices, Topology::TriangleList, false);
}

Rendering::FpsDisplay::~FpsDisplay()
{
	delete m_pMesh;
	for (int i = 0; i < NR_NUMBERS; i++)
		delete m_pTextures[i];
}

void Rendering::FpsDisplay::Render()
{
	m_Sampler.ActivatePs();
	m_RasterizerState.Activate();
	m_InputLayout.Activate();
	m_BlendState.Activate();
	m_Shader.Activate();

	const int numbers[]
	{
		m_Fps / 1000 % 10,
		m_Fps / 100 % 10,
		m_Fps / 10 % 10,
		m_Fps % 10
	};

	m_pMesh->Activate();
	bool began = false;
	float x = -1.f + m_ScreenSpaceMargin.x;
	for (int i = 0; i < 4; i++)
	{
		if (!began && numbers[i] == 0 && i != 3) continue;
		began = true;
		const Float2& size{ m_Sizes[numbers[i]] };
		SetPos(m_Vertices, { x, 1.f - m_ScreenSpaceMargin.y - size.y }, size);
		m_pMesh->Update(m_Vertices);

		m_pTextures[numbers[i]]->ActivatePs();
		m_pMesh->Draw();
		x += size.x;
	}
}

void Rendering::FpsDisplay::OnWindowResized(const Int2& newSize)
{
}

void Rendering::FpsDisplay::SetPos(Array<V_Pos2Uv>& vertices, const Float2& origin, const Float2& size)
{
	const float left{ origin.x };
	const float right{ left + size.x };
	const float bot{ origin.y };
	const float top{ bot + size.y };
	vertices[0].pos = { left, top };
	vertices[1].pos = { right, top };
	vertices[2].pos = { right, bot };
	vertices[3].pos = { left, top };
	vertices[4].pos = { right, bot };
	vertices[5].pos = { left, bot };
}

void Rendering::FpsDisplay::InitVertices(Array<V_Pos2Uv>& vertices)
{
	vertices[0] = { {},{0,0} };
	vertices[1] = { {},{1,0} };
	vertices[2] = { {},{1,1} };
	vertices[3] = { {},{0,0} };
	vertices[4] = { {},{1,1} };
	vertices[5] = { {},{0,1} };
}
