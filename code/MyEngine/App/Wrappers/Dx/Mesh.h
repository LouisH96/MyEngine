#pragma once
#include "Gpu.h"
#include "Shader.h"
#include "DxHelper.h"

namespace MyEngine
{
	namespace App
	{
		namespace Wrappers
		{
			namespace Dx
			{
				template<typename Vertex>
				class Mesh
				{
				public:
					Mesh(const Mesh& other) = delete;
					Mesh(Mesh&& other) noexcept = delete;
					Mesh& operator=(const Mesh& other) = delete;
					Mesh& operator=(Mesh&& other) noexcept = delete;

					explicit Mesh(Gpu& gpu, const Vertex* pVertices, int nrVertices, const int* pIndices, int nrIndices);
					~Mesh();

					void Activate() const;
					void Draw() const;

				private:
					Gpu& m_Gpu;

					ID3D11Buffer* m_pVertexBuffer{};
					ID3D11Buffer* m_pIndexBuffer{};
					unsigned int m_VertexCount{};
					unsigned int m_VertexStride{};
					unsigned int m_VertexOffset{};
					unsigned int m_IndexCount{};
				};

				template <typename Vertex>
				Mesh<Vertex>::Mesh(Gpu& gpu, const Vertex* pVertices, int nrVertices, const int* pIndices,
					int nrIndices)
					: m_Gpu(gpu)
					, m_VertexCount(nrVertices)
					, m_VertexStride(sizeof(Vertex))
					, m_VertexOffset(0)
					, m_IndexCount(nrIndices)
				{
					const D3D11_BUFFER_DESC vertexBufferDesc
					{
						static_cast<unsigned int>(sizeof(Vertex)) * m_VertexCount,
						D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER,
						0, 0, sizeof(Vertex)
					};

					const D3D11_SUBRESOURCE_DATA srData{ pVertices,0,0 };
					const HRESULT hr = m_Gpu.GetDevice().CreateBuffer(&vertexBufferDesc, &srData, &m_pVertexBuffer);
					if (FAILED(hr))
						throw std::exception("Mesh::InitVertexBuffer");

					DxHelper::CreateIndexBuffer(gpu.GetDevice(), m_pIndexBuffer, pIndices, nrIndices);
				}

				template <typename Vertex>
				Mesh<Vertex>::~Mesh()
				{
					SAFE_RELEASE(m_pIndexBuffer);
					SAFE_RELEASE(m_pVertexBuffer);
				}

				template <typename Vertex>
				void Mesh<Vertex>::Activate() const
				{
					m_Gpu.GetContext().IASetVertexBuffers(
						0,
						1,
						&m_pVertexBuffer,
						&m_VertexStride,
						&m_VertexOffset);
					m_Gpu.GetContext().IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
				}

				template <typename Vertex>
				void Mesh<Vertex>::Draw() const
				{
					m_Gpu.GetContext().DrawIndexed(m_IndexCount, 0, 0);
				}
			}
		}
	}
}
