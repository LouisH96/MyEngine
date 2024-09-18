#pragma once
#include "Rendering/State/BlendState.h"
#include "Rendering/State/ConstantBuffer.h"
#include "Rendering/State/InputLayout.h"
#include "Rendering/State/Mesh.h"
#include "Rendering/State/RasterizerState.h"
#include <Rendering/State/SamplerState.h>
#include <Rendering/State/Texture.h>

namespace MyEngine
{
namespace Rendering
{
//---| Class |---
template<typename Vertex, typename CamData>
class TextureRenderer
{
public:
	using VertexType = Vertex;
	using CamDataRefType = CamData;

	//---| Construction |---
	explicit TextureRenderer(const std::wstring& shaderPath);
	~TextureRenderer();

	//---| Rule of Five |---
	TextureRenderer(const TextureRenderer& other) = delete;
	TextureRenderer(TextureRenderer&& other) noexcept = delete;
	TextureRenderer& operator=(const TextureRenderer& other) = delete;
	TextureRenderer& operator=(TextureRenderer&& other) noexcept = delete;

	//---| Loop |---
	template<Shader::Function::Flag Function = Shader::Function::Both, bool UnsetOthers = true>
	void Render();

	//---| Operations |---
	int AddMesh(const Array<Vertex>& vertices, const Array<int>& indices, const std::wstring& texturePath);
	int AddMesh(const Array<Vertex>& vertices, const std::wstring& texturePath);
	int AddMesh(Mesh* pMesh, Texture* pTexture);

	void SetActive(int id, bool active);

	int GetNrMeshes() const { return m_Meshes.GetSize(); }

private:
	//---| Types |---
	struct MeshData
	{
		Mesh* pMesh{};
		Texture* pTexture{};
		bool Active{ true };
	};

	//---| General |---
	BlendState m_BlendState;
	RasterizerState m_RasterizerState;
	SamplerState m_Sampler;
	DepthStencilState m_DepthStencilState;

	//---| Mesh/Shader Specific |---
	static const InputLayout::Element ELEMENTS[];
	Shader m_Shader;
	InputLayout m_InputLayout;
	ConstantBuffer<CamData> m_ConstantBuffer;
	Array<MeshData> m_Meshes{};

	void Render_Internal();
};

template <typename Vertex, typename CamData>
TextureRenderer<Vertex, CamData>::TextureRenderer(const std::wstring& shaderPath)
	: m_Shader(shaderPath)
	, m_InputLayout(Vertex::ELEMENTS, Vertex::NR_ELEMENTS)
{
}

template <typename Vertex, typename CamData>
TextureRenderer<Vertex, CamData>::~TextureRenderer()
{
	for (unsigned i = 0; i < m_Meshes.GetSize(); i++)
	{
		delete m_Meshes[i].pMesh;
		delete m_Meshes[i].pTexture;
	}
}

template<typename Vertex, typename CamData>
template<Shader::Function::Flag Function, bool UnsetOthers>
inline void TextureRenderer<Vertex, CamData>::Render()
{
	m_Shader.Activate<Function, UnsetOthers>();
	Render_Internal();
}

template <typename Vertex, typename CamData>
void TextureRenderer<Vertex, CamData>::Render_Internal()
{
	m_DepthStencilState.Activate();
	m_Sampler.ActivatePs();
	m_ConstantBuffer.Update(CamData{ Globals::pCamera->GetPosition(), Globals::pCamera->GetViewProjection() });
	m_ConstantBuffer.Activate();
	m_RasterizerState.Activate();
	m_InputLayout.Activate();
	m_BlendState.Activate();
	for (unsigned i = 0; i < m_Meshes.GetSize(); i++)
	{
		if (!m_Meshes[i].Active) continue;
		m_Meshes[i].pTexture->ActivatePs();
		m_Meshes[i].pMesh->Activate();
		m_Meshes[i].pMesh->Draw();
	}
}

template <typename Vertex, typename CamData>
int TextureRenderer<Vertex, CamData>::AddMesh(const Array<Vertex>& vertices, const Array<int>& indices, const std::wstring& texturePath)
{
	Mesh* pMesh = Mesh::Create<Vertex>(vertices, indices);
	Texture* pTexture = new Texture(texturePath);
	m_Meshes.Add({ pMesh, pTexture });
	return m_Meshes.GetSize() - 1;
}

template <typename Vertex, typename CamData>
int TextureRenderer<Vertex, CamData>::AddMesh(const Array<Vertex>& vertices, const std::wstring& texturePath)
{
	Mesh* pMesh = Mesh::Create<Vertex>(vertices);
	Texture* pTexture = new Texture(texturePath);
	m_Meshes.Add({ pMesh, pTexture });
	return m_Meshes.GetSize() - 1;
}

template <typename Vertex, typename CamData>
int TextureRenderer<Vertex, CamData>::AddMesh(Mesh* pMesh, Texture* pTexture)
{
	m_Meshes.Add({ pMesh, pTexture });
	return m_Meshes.GetSize() - 1;
}

template <typename Vertex, typename CamData>
void TextureRenderer<Vertex, CamData>::SetActive(int id, bool active)
{
	m_Meshes[id].Active = active;
}
}
}
