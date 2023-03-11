#pragma once
#include "Gpu.h"
#include "Shader.h"
#include "DxHelper.h"
#include "DataStructures/Array.h"

namespace MyEngine
{
	namespace App
	{
		namespace Wrappers
		{
			namespace Dx
			{
				//---| Nested |---
				enum class Topology
				{
					TriangleList, LineStrip, Unknown
				};

				//---| Class |---
				template<typename Vertex>
				class Mesh
				{
				public:
					Mesh(const Mesh& other) = delete;
					Mesh(Mesh&& other) noexcept = delete;
					Mesh& operator=(const Mesh& other) = delete;
					Mesh& operator=(Mesh&& other) noexcept = delete;

					explicit Mesh(const Gpu& gpu, const Vertex* pVertices, int nrVertices, const int* pIndices, int nrIndices, Topology topology = Topology::TriangleList);
					explicit Mesh(const Gpu& gpu, const Ds::Array<Vertex>& vertices, const Ds::Array<int>& indices, Topology topology = Topology::TriangleList);
					explicit Mesh(const Gpu& gpu, const Ds::Array<Vertex>& vertices, Topology topology = Topology::TriangleList);
					~Mesh();

					void Activate() const;
					void Draw() const;
					void DrawNotIndexed() const;

				private:
					const Gpu& m_Gpu;

					ID3D11Buffer* m_pVertexBuffer{};
					ID3D11Buffer* m_pIndexBuffer{};
					unsigned int m_VertexCount{};
					unsigned int m_VertexStride{};
					unsigned int m_VertexOffset{};
					unsigned int m_IndexCount{};
					D3D11_PRIMITIVE_TOPOLOGY m_Topology{};

					static D3D11_PRIMITIVE_TOPOLOGY ToDxTopology(Topology topology);
				};

				template <typename Vertex>
				Mesh<Vertex>::Mesh(const Gpu& gpu, const Vertex* pVertices, int nrVertices, const int* pIndices,
					int nrIndices, Topology topology)
					: m_Gpu(gpu)
					, m_VertexCount(nrVertices)
					, m_VertexStride(sizeof(Vertex))
					, m_VertexOffset(0)
					, m_IndexCount(nrIndices)
					, m_Topology(ToDxTopology(topology))
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
				Mesh<Vertex>::Mesh(const Gpu& gpu, const Ds::Array<Vertex>& vertices, const Ds::Array<int>& indices, Topology topology)
					: Mesh<Vertex>(gpu, vertices.GetData(), vertices.GetSize(), indices.GetData(), indices.GetSize(), topology)
				{
				}

				template <typename Vertex>
				Mesh<Vertex>::Mesh(const Gpu& gpu, const Ds::Array<Vertex>& vertices, Topology topology)
					: Mesh<Vertex>(gpu, vertices, {}, topology)
				{
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
					m_Gpu.GetContext().IASetPrimitiveTopology(m_Topology);
				}

				template <typename Vertex>
				void Mesh<Vertex>::Draw() const
				{
					m_Gpu.GetContext().DrawIndexed(m_IndexCount, 0, 0);
				}

				template <typename Vertex>
				void Mesh<Vertex>::DrawNotIndexed() const
				{
					m_Gpu.GetContext().Draw(m_VertexCount, m_VertexOffset);
				}

				template <typename Vertex>
				D3D11_PRIMITIVE_TOPOLOGY Mesh<Vertex>::ToDxTopology(Topology topology)
				{
					switch(topology)
					{
					case Topology::TriangleList: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
					case Topology::LineStrip: return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
					case Topology::Unknown:
					default: return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
					}
				}
			}
		}
	}
}
