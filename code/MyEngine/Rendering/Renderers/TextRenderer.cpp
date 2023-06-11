#include "pch.h"
#include "TextRenderer.h"

#include <fstream>
#include "Framework/Resources.h"
#include "Io/Ttf/ContourOperations.h"
#include "Io/Ttf/TtfReader.h"
#include "Math/ScalarMath.h"
#include "Rendering/Canvas.h"
#include "Rendering/Image.h"
#include "Rendering/Font/FontAtlas.h"

Rendering::TextRenderer::TextRendererElementId::TextRendererElementId(int id)
	: m_Id{ id }
{
}

bool Rendering::TextRenderer::TextRendererElementId::operator==(const TextRendererElementId& other) const
{
	return m_Id == other.m_Id;
}

Rendering::TextRenderer::TextRenderer()
	: m_InputLayout{ InputLayout::FromType<Vertex>() }
	, m_Shader{ Framework::Resources::GetGlobalShaderPath(L"Font.hlsl") }
	, m_CanvasSize{ Globals::pCanvas->GetSize() }
{
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
	for (int i = 0; i < m_CenterBottomAnchoredIdx; i += VERTICES_PER_RECT)
	{
		const RectFloat rect
		{
			(m_Vertices[i].pos + Float2{1,1}).Scaled(scale) - Float2{1, 1},
			(m_Vertices[i + 5].pos - m_Vertices[i].pos).Scaled(scale)
		};
		Replace(i, rect);
	}
	for (int i = m_CenterBottomAnchoredIdx; i < m_Vertices.GetSize(); i += VERTICES_PER_RECT)
	{
		const RectFloat rect
		{
			{
				m_Vertices[i].pos.x * scale.x,
				(m_Vertices[i].pos.y + 1) * scale.y - 1
			},
			(m_Vertices[i + 5].pos - m_Vertices[i].pos).Scaled(scale)
		};
		Replace(i, rect);
	}
	m_CanvasSize = newSize;
}

void Rendering::TextRenderer::Render()
{
	m_InputLayout.Activate(*Globals::pGpu);
	m_FontAtlas.Activate(*Globals::pGpu);
	m_Shader.Activate();
	m_Vertices.Draw();
}

Rendering::TextRenderer::TextRendererElementId Rendering::TextRenderer::AddLeftBottom(const Int2& leftBot, float xWidth,
	const std::string& text, float spacing)
{
	const TextRendererElementId firstId{ m_CenterBottomAnchoredIdx / VERTICES_PER_RECT };
	const float yClip{ ToClipAlignMin(leftBot.y, m_CanvasSize.y) };

	const float uvWidthToScreen{ m_UvToScreen * xWidth };
	const float uvHeightToScreen{ m_HeightToWidth * uvWidthToScreen };
			 
	float x{ FloatMath::Cast(leftBot.x) };
	for (int i = 0; i < text.size(); i++)
	{
		if(text[i] == ' ')
		{
			x += m_SpaceWidth * uvWidthToScreen;
			continue;
		}

		const RectFloat charUvRect{ GetCharUvRect(text[i]) };
		const Float2 charScreenSize{
			charUvRect.GetWidth() * uvWidthToScreen,
			charUvRect.GetHeight()* uvHeightToScreen
		};
		const RectFloat charNdcRect{
			{
				ToClipAlignMin(x, m_CanvasSize.x),
				yClip
			},
			SizeToClip(charScreenSize, m_CanvasSize)
		};
		Add(m_CenterBottomAnchoredIdx, charNdcRect, charUvRect);
		m_CenterBottomAnchoredIdx += VERTICES_PER_RECT;
		x += charScreenSize.x + spacing;
	}
	return firstId;
}

Rendering::TextRenderer::TextRendererElementId Rendering::TextRenderer::AddCenterBottom(const Int2& leftBot,
	float xWidth, const std::string& text, float spacing)
{
	const TextRendererElementId firstId{ m_CenterBottomAnchoredIdx / VERTICES_PER_RECT };
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
		Add(m_Vertices.GetSize(), charNdcRect, charUvRect);
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
		if(c == ' ')
		{
			totalWidth += m_SpaceWidth;
			continue;
		}
		const float charUvWidth{ GetCharUvWidth(c) };
		const float charScreenWidth{ charUvWidth * uvWidthToScreen };
		totalWidth += charScreenWidth;
	}
	return totalWidth;
}

//Rendering::TextRenderer::TextRendererElementId Rendering::TextRenderer::AddCenterBottom(const RectInt& rectangle, const std::string& text, float spacing)
//{
//	const RectFloat rectFloat
//	{
//		{
//			ToClipAlignCenter(rectangle.GetLeft(), m_CanvasSize.x),
//			ToClipAlignMin(rectangle.GetBottom(), m_CanvasSize.y)
//		},
//		SizeToClip(rectangle.GetSize(), m_CanvasSize)
//	};
//	Add(m_Vertices.GetSize(), rectFloat);
//	return TextRendererElementId{ 1000 + (m_Vertices.GetSize() - m_CenterBottomAnchoredIdx) / VERTICES_PER_RECT - 1 };
//}

void Rendering::TextRenderer::Add(int idx, const RectFloat& rect, const RectFloat& uvRect)
{
	List<Vertex>& list{m_Vertices.GetList()};
	list.InsertEmpty(idx, VERTICES_PER_RECT);

	list[idx++] = { rect.GetLeftBot(), {uvRect.GetLeft(),uvRect.GetBottom()} };
	list[idx++] = { rect.GetLeftTop(), {uvRect.GetLeft(),uvRect.GetTopInv()} };
	list[idx++] = { rect.GetRightBot(), {uvRect.GetRight(),uvRect.GetBottom()} };

	list[idx++] = { rect.GetRightBot(), {uvRect.GetRight(),uvRect.GetBottom()} };
	list[idx++] = { rect.GetLeftTop(), {uvRect.GetLeft(),uvRect.GetTopInv()} };
	list[idx] = { rect.GetRightTop(), {uvRect.GetRight(),uvRect.GetTopInv()} };
}

void Rendering::TextRenderer::Replace(int idx, const RectFloat& rect)
{
	m_Vertices[idx++].pos = rect.GetLeftBot();
	m_Vertices[idx++].pos = rect.GetLeftTop();
	m_Vertices[idx++].pos = rect.GetRightBot();

	m_Vertices[idx++].pos = rect.GetRightBot();
	m_Vertices[idx++].pos = rect.GetLeftTop();
	m_Vertices[idx].pos = rect.GetRightTop();
}

int Rendering::TextRenderer::ToVertexIdx(TextRendererElementId id) const
{
	if (id.GetId() >= 1000)
		return (id.GetId() - 1000) * VERTICES_PER_RECT + m_CenterBottomAnchoredIdx;
	return id.GetId() * VERTICES_PER_RECT;
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
