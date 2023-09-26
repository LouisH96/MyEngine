#include "pch.h"
#include "Texture2DRenderer.h"

#include "Framework/Resources.h"
#include "Generation/PlaneGeneration.h"

Rendering::Texture2DRenderer::Texture2DRenderer()
	: m_InputLayout{ InputLayout::FromType<Instance>() }
	, m_Shader{ Resources::GlobalShader(L"Font_Inst.hlsl") }
{
}

void Rendering::Texture2DRenderer::Render()
{
	if (m_Textures.GetSize() == 0) return;
	m_DepthStencilState.Activate();
	m_BlendState.Activate();
	m_RasterizerState.Activate();
	m_SamplerState.Activate();

	m_InputLayout.Activate();
	m_Shader.Activate();

	for (int i = 0; i < m_Textures.GetSize(); i++)
	{
		m_Textures[i].Activate();
		m_Instances[i].Draw();
	}
}

int Rendering::Texture2DRenderer::Add(Texture&& texture)
{
	const Array<Vertex> vertices{ Generation::PlaneGeneration::CreateVertices({ 0,0 }, { 1,1 }, { {0,0},{1,1} })};
	m_Instances.Add({ vertices.GetData(), vertices.GetSize() });
	m_Textures.Add(std::move(texture));
	return m_Textures.GetSize() - 1;
}

void Rendering::Texture2DRenderer::Add(int idx, const Instance& instance)
{
	m_Instances[idx].Add(instance);
}

int Rendering::Texture2DRenderer::Add(Texture&& texture, const Instance& instance)
{
	const int idx{ Add(std::move(texture)) };
	Add(idx, instance);
	return idx;
}

Rendering::Texture& Rendering::Texture2DRenderer::GetTexture(int idx)
{
	return m_Textures[idx];
}
