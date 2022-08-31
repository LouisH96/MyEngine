#pragma once
#include "../IPainter.h"

namespace MyEngine
{
	namespace Gpu
	{
		namespace Dx
		{
			class DxShader;
			class DxMesh;
			class DxCanvas;

			class DxPainter final
				: public IPainter
			{
			public:
				void SetCanvas(ICanvas& canvas) override;
				void SetShader(IShader& shader) override;
				void SetMesh(IMesh& mesh) override;

				void BeginPaint() override;
				void EndPaint() override;
				void Paint() const override;

			private:
				DxCanvas* m_pCanvas{};
				DxShader* m_pShader{};
				DxMesh* m_pMesh{};
			};
		}
	}
}
