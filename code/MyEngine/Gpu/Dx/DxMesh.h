#pragma once

#include "DxDevice.h"
#include "../IMesh.h"

namespace MyEngine
{
	namespace Gpu
	{
		namespace Dx
		{
			class DxMesh final
				: public IMesh
			{
			public:
				DxMesh(const DxMesh& other) = delete;
				DxMesh(DxMesh&& other) noexcept = delete;
				DxMesh& operator=(const DxMesh& other) = delete;
				DxMesh& operator=(DxMesh&& other) noexcept = delete;

				explicit DxMesh(DxDevice& gpu);
				~DxMesh() override;

				void Activate() const;
				void Draw() const;

			private:
				DxDevice& m_Gpu;

				ID3D11Buffer* m_pVertexBuffer{};
				unsigned int m_VertexCount{};
				unsigned int m_VertexStride{};
				unsigned int m_VertexOffset{};
			};
		}
	}
}
