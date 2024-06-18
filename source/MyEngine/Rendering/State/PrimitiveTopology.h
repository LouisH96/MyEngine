#pragma once
#include <d3d11.h>

#include "Geometry/ModelTopology.h"

namespace MyEngine
{
	namespace Rendering
	{
		class PrimitiveTopology
		{
		public:
			static D3D11_PRIMITIVE_TOPOLOGY ToDx(ModelTopology modelTopology);
			static void Activate(D3D11_PRIMITIVE_TOPOLOGY topology);
			static void Activate(ModelTopology modelTopology);

			PrimitiveTopology() = default;
			explicit PrimitiveTopology(ModelTopology modelTopology);

			void Activate() const;

		private:
			D3D11_PRIMITIVE_TOPOLOGY m_DxTopology;
			ModelTopology m_ModelTopology;
		};
	}
}
