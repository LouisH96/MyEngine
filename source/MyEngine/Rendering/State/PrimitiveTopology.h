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
			template<ModelTopology TTopology>
			static constexpr D3D11_PRIMITIVE_TOPOLOGY ToDx();
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
		template<ModelTopology TTopology>
		inline constexpr D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology::ToDx()
		{
			if constexpr(TTopology == ModelTopology::LineList
				|| TTopology == ModelTopology::LineListIdx)
				return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

			if constexpr (TTopology == ModelTopology::LineStrip
				|| TTopology == ModelTopology::LineStripIdx)
				return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;

			if constexpr (TTopology == ModelTopology::TriangleList
				|| TTopology == ModelTopology::TriangleListIdx)
				return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

			if constexpr (TTopology == ModelTopology::TriangleStrip
				|| TTopology == ModelTopology::TriangleStripIdx)
				return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

			return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
		}
	}
}
