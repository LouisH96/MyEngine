#include "pch.h"
#include "NodeGraphFontRenderer.h"

#include "DataStructures/Adders/ListAdder.h"
#include "Framework/Resources.h"
#include "Rendering/Font/FontAtlas.h"

using namespace Applied;
using namespace Rendering;

NodeGraphFontRenderer::NodeGraphFontRenderer()
	: m_InputLayout{ InputLayout::FromType<Vertex>() }
	, m_Shader{ Resources::GlobalShader(L"NodeGraph_Tex.hlsl") }
	, m_RasterizerState{ false }
{
	Font::FontAtlas fontAtlas{ 64 };
	m_FontAtlas = Texture{ fontAtlas.GetImage() };
	m_Assembler = TextAssembler{ std::move(fontAtlas) };

	//
	const std::string text{ "Hallo paard" };

	List<Vertex> vertices{};
	m_Assembler.AssembleInto([](const Float2& position, const Float2& uv)
		{
			return Vertex{ position, {0,0,0},uv };
		}, ListAdder<Vertex>{vertices}, Float2{ 0,0 }, Float2{ 0,1 }, text, Float2{ 1.f });

	m_Vertices = Buffer<Vertex>{ vertices, true };
}

void NodeGraphFontRenderer::Render()
{
	m_RasterizerState.Activate();
	m_InputLayout.Activate();
	m_Shader.Activate();

	m_FontAtlas.ActivatePs();
	m_DepthStencilState.Activate();
	m_SamplerState.ActivatePs();
	m_BlendState.Activate();

	m_Vertices.Activate(0);
	m_Vertices.Draw();
}
