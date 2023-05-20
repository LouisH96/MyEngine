#include "pch.h"
#include "PrimitiveTopology.h"

D3D11_PRIMITIVE_TOPOLOGY Rendering::PrimitiveTopologyUtils::ToDx(PrimitiveTopology topology)
{
	switch (topology)
	{
	case PrimitiveTopology::TriangleList: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	case PrimitiveTopology::LineStrip: return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
	case PrimitiveTopology::LineList: return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	default:
	case PrimitiveTopology::Unknown:
		Logger::PrintError("Unknown PrimitiveTopology");
		return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
	}
}
