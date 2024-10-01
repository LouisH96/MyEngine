#include "pch.h"
#include "NewUiImageRenderer.h"

#include <Rendering\Canvas.h>

using namespace NewUi;
using namespace Rendering;

NewUiImageRenderer::NewUiImageRenderer()
	: m_Shader{ Resources::GlobalShader(L"screenSpace.hlsl") }
	, m_InputLayout{ InputLayout::FromType<Vertex>() }
	, m_2DivScreenSize{ Calc2InvScreenSize() }
	, m_Vertices{ sizeof(Vertex), 6, false, TOPOLOGY }
{
}

void NewUiImageRenderer::Clear()
{
	for (unsigned i{ 0 }; i < m_Groups.GetSize(); ++i)
		m_Groups[i].Vertices.Clear();
}

void NewUiImageRenderer::CreateBuffer()
{
	//Count
	unsigned nrVertices{ 0 };
	for (unsigned i{ 0 }; i < m_Groups.GetSize(); ++i)
		nrVertices += m_Groups[i].Vertices.GetSize();

	//Ensure Capacity
	if (nrVertices >= m_Vertices.GetCapacity())
			m_Vertices.SetCapacity(nrVertices + nrVertices / 2 + 1, false);

	//Update data
	Vertex* pVertex{ m_Vertices.BeginUpdateData() };
	for (unsigned iGroup{ 0 }; iGroup < m_Groups.GetSize(); ++iGroup)
	{
		const List<Vertex>& vertices{ m_Groups[iGroup].Vertices };
		for (unsigned iVertex{ 0 }; iVertex < vertices.GetSize(); ++iVertex)
			*pVertex++ = vertices[iVertex];
	}
	m_Vertices.EndUpdateData();
}

void NewUiImageRenderer::OnCanvasResized(const App::ResizedEvent&)
{
	m_2DivScreenSize = Calc2InvScreenSize();
}

void NewUiImageRenderer::Render()
{
	m_InputLayout.Activate();
	m_Shader.Activate();
	m_Vertices.Activate();

	unsigned iVertex{ 0 };
	for (unsigned iGroup{ 0 }; iGroup < m_Groups.GetSize(); ++iGroup)
	{
		m_Groups[iGroup].Texture.ActivatePs();
		const unsigned count{ m_Groups[iGroup].Vertices.GetSize() };
		m_Vertices.Draw(iVertex, count);
		iVertex += count;
	}
}

void NewUiImageRenderer::Add(const RectFloat& screenSpace, const Float2& uvLeftTop, const Float2& uvRightBot, unsigned textureId)
{
	TextureGroup& group{ m_Groups[textureId] };
	const RectFloat ndcRect{
		screenSpace.GetLeftBot() * m_2DivScreenSize - Float2{1},
		screenSpace.GetSize() * m_2DivScreenSize };

	//Triangle: LeftTop
	const Vertex leftTop{ ndcRect.GetLeftTop(), uvLeftTop };
	group.Vertices.Add(leftTop);

	const Vertex rightTop{ ndcRect.GetRightTop(), {uvRightBot.x,uvLeftTop.y} };
	const Vertex leftBot{ ndcRect.GetLeftBot(), {uvLeftTop.x,uvRightBot.y} };
	group.Vertices.Add(rightTop);
	group.Vertices.Add(leftBot);

	//Triangle: RightBot
	group.Vertices.Add(leftBot);
	group.Vertices.Add(rightTop);
	const Vertex rightBot{ ndcRect.GetRightBot(), uvRightBot };
	group.Vertices.Add(rightBot);
}

Float2 NewUiImageRenderer::Calc2InvScreenSize()
{
	return Float2{ 2 } / Globals::pCanvas->GetSize();
}

unsigned NewUiImageRenderer::AddTexture(Rendering::Texture&& texture)
{
	m_Groups.Add(TextureGroup{ std::move(texture), 0 });
	return m_Groups.GetSize() - 1;
}

void NewUiImageRenderer::ReplaceTexture(unsigned textureId, Rendering::Texture&& texture)
{
	m_Groups[textureId].Texture = std::move(texture);
}
