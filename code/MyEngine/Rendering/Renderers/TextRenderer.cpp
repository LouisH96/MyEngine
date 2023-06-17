#include "pch.h"
#include "TextRenderer.h"

#include <fstream>
#include "Framework/Resources.h"
#include "Generation/PlaneGeneration.h"
#include "Io/Ttf/TtfReader.h"
#include "Math/Scalar.h"
#include "Rendering/Canvas.h"
#include "Rendering/Image.h"
#include "Rendering/Font/FontAtlas.h"

Rendering::TextRenderer::TextRendererElementId::TextRendererElementId(unsigned id)
	: m_Id{ static_cast<int>(id) }
{
}

bool Rendering::TextRenderer::TextRendererElementId::operator==(const TextRendererElementId& other) const
{
	return m_Id == other.m_Id;
}

Rendering::TextRenderer::TextRenderer()
	: m_InputLayout{ InputLayout::FromType<Instance>() }
	, m_Shader{ Resources::GlobalShader(L"Font_Inst.hlsl") }
	, m_DepthStencilState{ false }
	, m_CanvasSize{ Globals::pCanvas->GetSize() }
{
	//VERTICES
	const Array<Vertex> vertices{ Generation::PlaneGeneration::CreateVertices({ 0,0 }, { 1,1 }, { {0,0},{1,1} })};
	m_Instances = InstanceList<Vertex, Instance>{
		vertices.GetData(), vertices.GetSizeU()
	};

	//ATLAS
	Font::FontAtlas fontAtlas{32};
	m_FontAtlas = Texture{ fontAtlas.GetImage() };
	m_HeightToWidth = FloatMath::Cast(fontAtlas.GetImage().GetHeight()) / FloatMath::Cast(fontAtlas.GetImage().GetWidth());
	m_CharactersHeight = std::move(fontAtlas.GetCharacterHeight());
	m_CharactersHorPos = std::move(fontAtlas.GetCharacterHorPos());
	m_UvToScreen = 1.f / GetCharUvWidth('x');
	m_SpaceWidth = fontAtlas.GetSpaceWidth();
}

void Rendering::TextRenderer::OnCanvasResize(const Int2& newSize)
{
	const Float2 scale
	{
		static_cast<float>(m_CanvasSize.x) / newSize.x,
		static_cast<float>(m_CanvasSize.y) / newSize.y
	};

	//left-bottom
	for (int i = 0; i < m_CenterBottomAnchoredIdx; i++)
	{
		const RectFloat rect
		{
			(m_Instances[i].posOffset + Float2{1,1}).Scaled(scale) - Float2{1, 1},
			m_Instances[i].posScale.Scaled(scale)
		};
		Replace(i, rect);
	}
	for (int i = m_CenterBottomAnchoredIdx; i < m_Instances.GetSize(); i++)
	{
		const RectFloat rect
		{
			{
				m_Instances[i].posOffset.x * scale.x,
				(m_Instances[i].posOffset.y + 1) * scale.y - 1
			},
			m_Instances[i].posScale.Scaled(scale)
		};
		Replace(i, rect);
	}
	m_CanvasSize = newSize;
}

void Rendering::TextRenderer::Render()
{
	if (m_Instances.GetSize() == 0) return;

	m_DepthStencilState.Activate();
	m_BlendState.Activate();
	m_RasterizerState.Activate();
	m_SamplerState.Activate();
	m_InputLayout.Activate();
	m_FontAtlas.Activate();
	m_Shader.Activate();

	m_Instances.Draw();
}

Rendering::TextRenderer::TextRendererElementId Rendering::TextRenderer::AddLeftBottom(const Int2& leftBot, float xWidth,
	const std::string& text, const Float3& color, float spacing)
{
	const TextRendererElementId firstId{ m_CenterBottomAnchoredIdx };
	const float yClip{ ToClipAlignMin(leftBot.y, m_CanvasSize.y) };

	const float uvWidthToScreen{ m_UvToScreen * xWidth };
	const float uvHeightToScreen{ m_HeightToWidth * uvWidthToScreen };

	float x{ FloatMath::Cast(leftBot.x) };
	for (int i = 0; i < text.size(); i++)
	{
		if (text[i] == ' ')
		{
			x += m_SpaceWidth * uvWidthToScreen;
			continue;
		}

		const RectFloat charUvRect{ GetCharUvRect(text[i]) };
		const Float2 charScreenSize{
			charUvRect.GetWidth() * uvWidthToScreen,
			charUvRect.GetHeight() * uvHeightToScreen
		};
		const RectFloat charNdcRect{
			{
				ToClipAlignMin(x, m_CanvasSize.x),
				yClip
			},
			SizeToClip(charScreenSize, m_CanvasSize)
		};
		Add(m_CenterBottomAnchoredIdx, charNdcRect, charUvRect, color);
		m_CenterBottomAnchoredIdx++;
		x += charScreenSize.x + spacing;
	}
	return firstId;
}

Rendering::TextRenderer::TextRendererElementId Rendering::TextRenderer::AddCenterBottom(const Int2& leftBot,
	float xWidth, const std::string& text, const Float3& color, float spacing)
{
	const TextRendererElementId firstId{ m_Instances.GetSize() };
	const float yClip{ ToClipAlignMin(leftBot.y, m_CanvasSize.y) };

	const float uvWidthToScreen{ m_UvToScreen * xWidth };
	const float uvHeightToScreen{ m_HeightToWidth * uvWidthToScreen };

	float x{ FloatMath::Cast(leftBot.x) };
	for (int i = 0; i < text.size(); i++)
	{
		if (text[i] == ' ')
		{
			x += m_SpaceWidth * uvWidthToScreen;
			continue;
		}

		const RectFloat charUvRect{ GetCharUvRect(text[i]) };
		const Float2 charScreenSize{
			charUvRect.GetWidth() * uvWidthToScreen,
			charUvRect.GetHeight() * uvHeightToScreen
		};
		const RectFloat charNdcRect{
			{
				ToClipAlignCenter(x, m_CanvasSize.x),
				yClip
			},
			SizeToClip(charScreenSize, m_CanvasSize)
		};
		Add(m_Instances.GetSize(), charNdcRect, charUvRect, color);
		x += charScreenSize.x + spacing;
	}
	return firstId;
}

float Rendering::TextRenderer::GetScreenWidth(float xWidth, const std::string& text, float spacing) const
{
	const float uvWidthToScreen{ m_UvToScreen * xWidth };

	float totalWidth{ 0 };
	for (const char c : text)
	{
		if (c == ' ')
		{
			totalWidth += m_SpaceWidth;
			continue;
		}
		const float charUvWidth{ GetCharUvWidth(c) };
		const float charScreenWidth{ charUvWidth * uvWidthToScreen };
		totalWidth += charScreenWidth + spacing;
	}
	return totalWidth;
}

void Rendering::TextRenderer::Add(int idx, const RectFloat& rect, const RectFloat& uvRect, const Float3& color)
{
	const Instance instanceData{
		rect.GetLeftBot(), rect.GetSize(),
		uvRect.GetSize(), uvRect.GetLeftBot(),
		color
	};
	m_Instances.Insert(instanceData, idx);
}

void Rendering::TextRenderer::Replace(int idx, const RectFloat& rect)
{
	m_Instances[idx].posOffset = rect.GetLeftBot();
	m_Instances[idx].posScale = rect.GetSize();
}

int Rendering::TextRenderer::ToVertexIdx(TextRendererElementId id) const
{
	if (id.GetId() >= 1000)
		return (id.GetId() - 1000) + m_CenterBottomAnchoredIdx;
	return id.GetId();
}

float Rendering::TextRenderer::GetCharUvWidth(int charIdx) const
{
	return m_CharactersHorPos[charIdx + 1] - m_CharactersHorPos[charIdx];
}

float Rendering::TextRenderer::GetCharUvWidth(char c) const
{
	return GetCharUvWidth(CharacterToIndex(c));
}

Float2 Rendering::TextRenderer::GetCharUvSize(int charIdx) const
{
	return { GetCharUvWidth(charIdx) , m_CharactersHeight[charIdx] };
}

Float2 Rendering::TextRenderer::GetCharUvSize(char c) const
{
	return GetCharUvSize(CharacterToIndex(c));
}

RectFloat Rendering::TextRenderer::GetCharUvRect(char c) const
{
	return GetCharUvRect(CharacterToIndex(c));
}

RectFloat Rendering::TextRenderer::GetCharUvRect(int charIdx) const
{
	RectFloat rect{ {m_CharactersHorPos[charIdx], m_CharactersHeight[charIdx]}, {m_CharactersHorPos[charIdx + 1], m_CharactersHeight[charIdx]} };
	rect.SetWidth(rect.GetWidth() - rect.GetLeft());
	return rect;
}

float Rendering::TextRenderer::ToClipAlignMin(int screenPos, float screenSize)
{
	return static_cast<float>(screenPos) / screenSize * 2 - 1;
}

float Rendering::TextRenderer::ToClipAlignCenter(int screenPos, float screenSize)
{
	return static_cast<float>(screenPos) / screenSize * 2;
}

float Rendering::TextRenderer::SizeToClip(float size, float screenSize)
{
	return size / screenSize * 2;
}

float Rendering::TextRenderer::SizeToClip(int size, float screenSize)
{
	return static_cast<float>(size) / screenSize * 2;
}

Float2 Rendering::TextRenderer::SizeToClip(const Float2& size, const Float2& screenSize)
{
	return {
		size.x / screenSize.x * 2,
		size.y / screenSize.y * 2
	};
}

Float2 Rendering::TextRenderer::SizeToClip(const Int2& size, const Float2& screenSize)
{
	return {
		static_cast<float>(size.x) / screenSize.x * 2,
		static_cast<float>(size.y) / screenSize.y * 2
	};
}

int Rendering::TextRenderer::CharacterToIndex(char c)
{
	if (c >= 'a') return c - 'a';
	if (c >= 'A') return 'z' - 'a' + 1 + c - 'A';
	return ('z' - 'a' + 1) * 2 + c - '0';
}
