#pragma once
#include "Gpu.h"
#include "Shader.h"

namespace MyEngine
{
	namespace App
	{
		namespace Wrappers
		{
			namespace Gpu
			{
				class Mesh
				{
				public:
					Mesh(const Mesh& other) = delete;
					Mesh(Mesh&& other) noexcept = delete;
					Mesh& operator=(const Mesh& other) = delete;
					Mesh& operator=(Mesh&& other) noexcept = delete;

					explicit Mesh(Gpu& gpu, const Shader::Vertex* pVertices, int nrVertices, const int* pIndices, int nrIndices);
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

					ID3D11RasterizerState* m_pRasterizerState{};
					void InitRasterizerState();
					void ReleaseRasterizerState();
				};
			}
		}
	}
}
