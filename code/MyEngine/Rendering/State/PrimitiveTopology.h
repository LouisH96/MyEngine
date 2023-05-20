#pragma once
#include <d3d11.h>

namespace MyEngine
{
	namespace Rendering
	{
		enum class PrimitiveTopology
		{
			TriangleList, LineStrip, LineList, Unknown
		};

		class PrimitiveTopologyUtils
		{
		public:
			static  D3D11_PRIMITIVE_TOPOLOGY ToDx(PrimitiveTopology topology);
		};
	}
}
